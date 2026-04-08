#include "LPC11xx.h"
#include "Delay.h"
#include "driver_config.h"
#include "target_config.h"
#include "timer32.h"
#include "timer16.h"
#include "gpio.h"
#include "LED.h"
#include "Motor.h"
#include "ssp.h"
#include "NRF24L01.h"
#include "UARTPrintf.h"
#include "stdio.h"
#include "OLED.h"
#include "Track.h"
#include "uart.h"
#include "i2c.h"
#include "PID.h"
#include "rtthread.h"
#include "Key.h"
#include "Ultrasonic.h"


/* 定义三个线程的控制块和栈空间 */
static struct rt_thread track_move_thread;
static struct rt_thread oled_display_thread;
static struct rt_thread comm_thread;
static rt_uint8_t track_move_thread_stack[1024];
static rt_uint8_t oled_display_thread_stack[1024];
static rt_uint8_t comm_thread_stack[1024];

extern volatile uint8_t key_flag;


extern uint8_t NRF24L01_TxAddress[];
extern uint8_t NRF24L01_TxPacket[];
extern uint8_t NRF24L01_RxAddress[];
extern uint8_t NRF24L01_RxPacket[];
uint8_t SendFlag;								//发送标志位
uint8_t SendSuccessCount, SendFailedCount;		//发送成功计次，发送失败计次
uint8_t ReceiveFlag;							//接收标志位
uint8_t ReceiveSuccessCount, ReceiveFailedCount;   //接收成功计次，接收失败计次



#define BASE_SPEED        50.0f   // 直道基础速度
#define MAX_SPEED        80.0f   // 直道基础速度

// 全局变量：循迹状态、历史备份及错误计数
extern volatile float track_status[1];               // 当前循迹偏差值
extern volatile float track_status_backup[1][20];    // 循迹状态历史备份（20个历史值）
extern volatile uint32_t track_status_worse;         // 循迹异常状态计数器
extern volatile _track_state track_state;            // 循迹状态联合体

// 全局PID句柄（用于循迹转向控制）
PID_HandleTypeDef track_pid = {0};
volatile float move_flag = 0;
volatile int left_speed = 0;
volatile int right_speed = 0;

// 新增：二号→三号的准备指令（通知三号做好准备）
#define CAR3_READY_CMD 0xBA  
// 新增：发送三号准备指令的标志（避免重复发送）
static uint8_t send_car3_ready_flag = 0;  

// 循迹运行状态枚举
typedef enum {
	// 新增：初始阶段（闭环前置逻辑）
	STATE_INIT_FIND_BLACK,        // 前进找黑线
    STATE_INIT_FORWARD,          // 初始直线前进（计时）
    STATE_INIT_TURN_RIGHT,       // 原地右转调整方向（计时）
    STATE_INIT_TRACK_TO_START,   // 巡线到起点十字交叉口
    STATE_WAIT_CAR2_CMD,         // 等待二号小车启动指
	
    STATE_STOP_INIT,         // 初始状态：停在十字交叉路口（全黑0x1F）
    STATE_ROTATE_CCW,        // 逆时针绕圈状态
    STATE_PARKING_AREA,      // 进入停车区（检测到第一个十字路口）
    STATE_COUNT_PARK_FRAME,  // 计数停车框（0-4个）
    STATE_TURN_RIGHT,        // 右转向状态
    STATE_PARK_FINISH        // 停车完成（再次检测到全黑）
} RunState;

// 全局状态变量及停车框计数器
static RunState current_state = STATE_INIT_FIND_BLACK;
static uint8_t park_frame_count = 0;  // 停车框计数（0-4）

// 新增：小车通信控制标志（确保启动指令只发送一次）
static uint8_t send_to_car2_flag = 0;  // 1=已向二号小车发送启动指令，0=未发送
   
// 新增：二号车准备指令（一号→二号，到WAIT状态时发，通知出库）
#define CAR2_READY_CMD 0xAA      
// 新增标志位：避免重复发送准备指令（全局静态）
static uint8_t send_car2_ready_flag = 0; 
// 原有：二号车启动巡线指令（一号→二号，到停车区时发）
#define CAR2_START_CMD 0xAB        

// OLED显示控制相关（通信线程→OLED线程的通知标志）
static uint8_t oled_update_flag = 0;   // 1=需要更新OLED显示，0=无需更新
static char oled_msg_line4[16] = {0};  // 第四行显示的消息缓冲区


// 新增宏定义（放在RunState枚举下方，方便调试）
#define PARK_FRAME_DETECT_THRESHOLD 2    // 连续检测3次=真检测到停车框
#define PARK_FRAME_LOCK_TIME       60  // 计数后锁定200ms，避免重复计数
#define TICK_TO_MS                 1     // RT-Thread默认1tick=1ms（无需修改）

// 新增静态变量（放在current_state和park_frame_count下方，仅STATE_COUNT_PARK_FRAME使用）
static uint8_t  park_frame_detect_cnt = 0;  // 连续检测到0x1C的次数
static rt_tick_t park_frame_last_tick = 0;  // 上一次有效计数的系统节拍（时间戳）

// 右转超时兜底配置（可根据实际调试调整，比如2秒）
#define TURN_RIGHT_TIMEOUT_MS 1250  // 右转最长时间（毫秒），超时强制停车
static rt_tick_t turn_right_start_tick = 0;  // 右转状态开始的系统节拍（时间戳）

// 新增：找黑线相关配置
#define INIT_FIND_BLACK_SPEED     35      // 找黑线时的低速（BASE_SPEED*0.5）
#define INIT_FORWARD_AFTER_BLACK_MS 200  // 找到黑线后再前进200ms
static uint8_t  find_black_flag = 0;     // 1=已找到黑线
static rt_tick_t forward_after_black_tick = 0; // 找到黑线后前进的计时戳

// 新增：初始阶段计时变量（仅新增）
//#define INIT_FORWARD_TIME_MS    1000     // 初始直线前进时间（可调试）
#define INIT_TURN_RIGHT_TIME_MS 650      // 原地右转时间（可调试）
static rt_tick_t init_forward_start_tick = 0;   // 直线前进计时戳
static rt_tick_t init_turn_right_start_tick = 0;// 原地右转计时戳
static uint8_t wait_car2_cmd_flag = 0; // 接收二号车启动指令标志

// 新增：发送三号准备指令的延时（单位：ms，根据需求调整，比如500ms）
#define DETECT_DELAY_MS  2000  
static rt_tick_t detect_start_tick = 0;      // 计时起始时间戳（RT-Thread节拍）

void track_move_control(void) {
    move_flag = track_status[0];
	    // 限幅处理：将move_flag限制在[-4, 4]
    if (move_flag > 20.0f) {
        move_flag = 20.0f;  // 超过上限，强制设为4
    } else if (move_flag < -20.0f) {
        move_flag =  -20.0f;  // 低于下限，强制设为-4
    }
	    // 2. 计算PID输出（转向控制量）
    pid_calculate(&track_pid, move_flag);
	    // 3. 根据PID输出计算左右电机速度（输出越大，转向越急）
    // 左电机速度 = 基础速度 - 转向量（转向量为正：左转，左慢右快）
    // 右电机速度 = 基础速度 + 转向量（转向量为负：右转，左快右慢）
	if (move_flag == 0) {
		track_pid.integral = 0;
		track_pid.output = 0;
	}
    // 状态机逻辑
    switch (current_state) {
		    // 新增：第一步 - 前进找黑线
		case STATE_INIT_FIND_BLACK:
			if(key_flag == 1){
				// key_flag = 0;
				// 低速前进找黑线（左右轮同速）
				left_speed = 35;
				right_speed = 35;
				// 检测是否找到黑线（根据实际循迹逻辑，比如track_state.state非全白/非0）
				// 需根据你的track_state定义调整，比如：
				// 黑线检测条件：track_state.state != 0（全白） 或 任意红外管检测到黑线
				if (track_state.state == 0x1F) { // 找到黑线（替换为你实际的黑线检测条件）
					find_black_flag = 1;
					current_state = STATE_INIT_FORWARD; // 切换到“再走一点”
					forward_after_black_tick = rt_tick_get(); // 初始化计时
				}
			}
			break;
		
		        // 新增：初始直线前进（仅新增此case）
        case STATE_INIT_FORWARD:
			if (find_black_flag == 1){
				// 继续低速前进
				left_speed = INIT_FIND_BLACK_SPEED;
				right_speed = INIT_FIND_BLACK_SPEED;
				// 计时到指定时间后，切换到原地右转
				if ((rt_tick_get() - forward_after_black_tick) >= 200) {  // S=VT  往前走200ms
					current_state = STATE_INIT_TURN_RIGHT; // 切到原有右转逻辑
					find_black_flag = 0; // 重置标志
					forward_after_black_tick = 0; // 重置计时
				}
			}
            break;

        // 新增：原地右转（仅新增此case）
        case STATE_INIT_TURN_RIGHT:
            if (init_turn_right_start_tick == 0) {
                init_turn_right_start_tick = rt_tick_get();
            }
            // 原地右转：左轮快、右轮慢/反转（根据实际调试）
            left_speed = (int)(35);
            right_speed = (int)(-35); // 反转或设0.2，看电机驱动
            // 计时到切换到巡线到起点
            if ((rt_tick_get() - init_turn_right_start_tick) >= 650) {
                current_state = STATE_INIT_TRACK_TO_START;
                init_turn_right_start_tick = 0;
            }
            break;

        // 新增：巡线到起点（仅新增此case）
        case STATE_INIT_TRACK_TO_START:
            // 复用原有PID循迹逻辑
            left_speed = (int)(BASE_SPEED * 0.65 + track_pid.output);
            right_speed = (int)(BASE_SPEED * 0.65 - track_pid.output);
            if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
            if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
            // 检测到起点十字（0x1F）切换到等待指令
            if (track_state.state == 0x1F) {
//				rt_thread_mdelay(20);
                current_state = STATE_WAIT_CAR2_CMD;
//                left_speed = 0;
//                right_speed = 0;
            }
            break;
		
		// 新增：等待二号车指令（仅新增此case）
        case STATE_WAIT_CAR2_CMD:
//            left_speed = 0;
//            right_speed = 0;
		    // 仅首次进入该状态时，发送「二号车准备指令」（避免重复发）
			if (send_car2_ready_flag == 0) {
				send_car2_ready_flag = 1; // 标记已触发发送
				// 触发通信线程发送准备指令（也可直接在这调用发送，推荐通过标志位解耦）
				// 注：通信线程需监听该标志位
				current_state = STATE_STOP_INIT; // 衔接原有逻辑
				detect_start_tick = rt_tick_get();
//				oled_update_flag = 1;
//				sprintf(oled_msg_line4, "Car2 Ready CMD");
			}

            break;
			
        case STATE_STOP_INIT:
            // 初始状态：停在十字路口（全黑0x1F），等待按键
//            left_speed = 0;
//            right_speed = 0;
//            if(key_flag == 1){
                current_state = STATE_ROTATE_CCW;  // 切换到逆时针绕圈
                // rt_kprintf("进入逆时针绕圈状态\n");
//            }
            break;

        case STATE_ROTATE_CCW:
            // 逆时针绕圈：根据循迹状态调整方向（沿用原有PID控制）
            left_speed = (int)(BASE_SPEED + track_pid.output);
            right_speed = (int)(BASE_SPEED - track_pid.output);
            // 限制最大速度
            if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
            if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
			if(rt_tick_get() - detect_start_tick >= DETECT_DELAY_MS){
				// 检测到十字路口（全黑0x1F），进入停车区
				if (track_state.state == 0x1F) {
					current_state = STATE_PARKING_AREA;
					// rt_kprintf("到达停车区，开始计数停车框\n");
				}
			}
            break;

        case STATE_PARKING_AREA:
            // 进入停车区：过渡到计数停车框状态
			send_to_car2_flag = 1;
			park_frame_count = 0;  // 重置停车框计数
            current_state = STATE_COUNT_PARK_FRAME;
            break;

		// 修改STATE_COUNT_PARK_FRAME分支逻辑
		case STATE_COUNT_PARK_FRAME:
			// 保持电机正常控制（无阻塞）
			left_speed = (int)(BASE_SPEED + track_pid.output);
			right_speed = (int)(BASE_SPEED - track_pid.output);

			// 步骤1：判断是否在「计数锁定期」内（避免短时间重复计数）
			rt_tick_t current_tick = rt_tick_get();  // 获取当前系统节拍（1ms/tick）
			if ((current_tick - park_frame_last_tick) < (PARK_FRAME_LOCK_TIME / TICK_TO_MS)) {
				// 锁定期内，重置连续检测计数，直接跳过
				park_frame_detect_cnt = 0;
				break;
			}

			// 步骤2：连续检测停车框（0x1C）  0x18
			if (track_state.state == 0x1C || track_state.state == 0x18) {
				park_frame_detect_cnt++;  // 连续检测次数+1
				// 步骤3：达到连续检测阈值，判定为有效停车框
				if (park_frame_detect_cnt >= PARK_FRAME_DETECT_THRESHOLD) {
					park_frame_count++;  // 停车框计数+1
					park_frame_last_tick = current_tick;  // 记录本次计数时间戳
					park_frame_detect_cnt = 0;  // 重置连续检测计数
					// rt_kprintf("有效检测到第%d个停车框\n", park_frame_count);
				}
			} else {
				// 未检测到0x1C，重置连续检测计数（过滤单次误触发）
				park_frame_detect_cnt = 0;
			}

			// 步骤4：计数到4个停车框，进入右转状态
			if (park_frame_count >= 4) {
				current_state = STATE_TURN_RIGHT;
				park_frame_count = 0;          // 重置计数（防止再次触发）
				park_frame_detect_cnt = 0;     // 重置连续检测计数
				park_frame_last_tick = 0;      // 重置锁定期
				// rt_kprintf("4个停车框已通过，准备右转\n");
			}
			break;

			case STATE_TURN_RIGHT:
				// send_car3_ready_flag = 1;
				// 首次进入右转状态时，记录开始计时的时间戳（仅初始化一次）
				if (turn_right_start_tick == 0) {
					turn_right_start_tick = rt_tick_get();  // 记录右转开始时间
					// rt_kprintf("进入右转状态，开始计时兜底\n");
				}
		
				// 右转向控制（可根据实际需求调整转速差实现右转）
				left_speed = (int)(BASE_SPEED * 1.35);  // 左轮快，右轮慢实现右转
				right_speed = (int)(BASE_SPEED * 0.4);
				// 逻辑1：优先检测到全黑十字路口（0x1F），立即停车
				if (track_state.state == 0x1F) {
					current_state = STATE_PARK_FINISH;
					turn_right_start_tick = 0;  // 重置计时（避免下次进入时残留）
					// rt_kprintf("检测到全黑0x1F，右转停车\n");
				}
				// 逻辑2：未检测到0x1F，判断是否超时（兜底逻辑）
				else {
					rt_tick_t current_tick = rt_tick_get();
					// 计算已右转时间：当前tick - 开始tick（RT-Thread默认1tick=1ms）
					if ((current_tick - turn_right_start_tick) >= TURN_RIGHT_TIMEOUT_MS) {
						current_state = STATE_PARK_FINISH;
						turn_right_start_tick = 0;  // 重置计时
						// rt_kprintf("右转超时（%dms），强制停车\n", TURN_RIGHT_TIMEOUT_MS);
					}
				}
				break;

        case STATE_PARK_FINISH:
            // 停车完成，停止运动
            left_speed = 0;
            right_speed = 0;
            // rt_kprintf("停车成功\n");
            break;

        default:
            left_speed = 0;
            right_speed = 0;
            break;
    }

	left_motor(left_speed);
	right_motor(right_speed);
}



static void track_move_entry(void* parameter)
{
	pid_init(&track_pid);
	motor_pwm_init();
	
    while (1)
    {
		track_check();
		track_move_control();
        rt_thread_mdelay(10);
    }
}



static void comm_thread_entry(void* parameter)
{
	
	NRF24L01_Init();    //NRF24L01初始化
		/* 初始化发送数据 */
	NRF24L01_TxPacket[0] = 0x00;
	NRF24L01_TxPacket[1] = 0x00;
	NRF24L01_TxPacket[2] = 0x00;
	NRF24L01_TxPacket[3] = 0x00;
	
    while (1)
    {
		        // 新增：发送「二号车准备指令」（仅1次）
        if (send_car2_ready_flag == 1) {
            NRF24L01_TxPacket[0] = CAR2_READY_CMD; // 准备指令0xAD
            SendFlag = NRF24L01_Send();
            if (SendFlag == 1) {
                SendSuccessCount++;
                send_car2_ready_flag = 2; // 标记发送成功（避免重复）
                sprintf(oled_msg_line4, "Car2 Ready OK");
            } else {
                SendFailedCount++;
                sprintf(oled_msg_line4, "Car2 Ready Fail");
            }
            oled_update_flag = 1;
        }
		
        /************************ 核心逻辑：检测停车区并发送启动指令 ************************/
        // 条件：进入停车区状态 + 未向二号小车发送过启动指令
        if (send_to_car2_flag == 1)
        {
            // 1. 构造给二号小车的启动指令数据包
            NRF24L01_TxPacket[0] = CAR2_START_CMD;  // 第0字节：启动指令（0x01）
            NRF24L01_TxPacket[1] = 0x00;            // 预留字节（可扩展）
            NRF24L01_TxPacket[2] = 0x00;
            NRF24L01_TxPacket[3] = 0x00;

            // 2. 发送启动指令
            SendFlag = NRF24L01_Send();
            
            // 3. 处理发送结果，并通知OLED显示
            if (SendFlag == 1) {            // 发送成功
                SendSuccessCount++;
                send_to_car2_flag = 1;     // 标记为已发送，避免重复
                // 填充第四行成功消息
                sprintf(oled_msg_line4, "Send Car2 OK");
                oled_update_flag = 1;      // 通知OLED线程更新显示
            } else {                        // 发送失败
                SendFailedCount++;
                // 填充第四行失败消息
                sprintf(oled_msg_line4, "Send Car2 Fail");
                oled_update_flag = 1;      // 通知OLED线程更新显示
            }
        }
		
//		/************************ 新增：第二步 - 发送三号小车准备指令（0xBA） ************************/
//        if (send_car3_ready_flag == 1) {
//            // 构造三号准备指令（0xBA）
//            NRF24L01_TxPacket[0] = 0x00; // 准备指令0xBA
//            NRF24L01_TxPacket[1] = CAR3_READY_CMD;
//            NRF24L01_TxPacket[2] = 0x00;
//            NRF24L01_TxPacket[3] = 0x00;

//            // 发送指令
//            SendFlag = NRF24L01_Send();
//            if (SendFlag == 1) {            // 发送成功
//                SendSuccessCount++;
//                send_car3_ready_flag = 2;  // 标记已发送（避免重复）
////                sprintf(oled_msg_line4, "Car3 Ready OK");
////                oled_update_flag = 1;
//            } else {                        // 发送失败
//                SendFailedCount++;
////                sprintf(oled_msg_line4, "Car3 Ready Fail");
////                oled_update_flag = 1;
//				send_car3_ready_flag = 1;
//                // 失败后可重试（可选）：send_car3_ready_flag 保持1，下次循环再发
//            }
//        }
		
        rt_thread_mdelay(50);
    }
}

// 辅助函数：将状态机枚举转换为可读字符串（方便显示）
static const char* get_state_string(RunState state)
{
    switch(state)
    {
		        // 新增：4个初始状态的映射
		case STATE_INIT_FIND_BLACK:    return "Init Find Black";
        case STATE_INIT_FORWARD:      return "Init Forward";
        case STATE_INIT_TURN_RIGHT:   return "Init Turn R";
        case STATE_INIT_TRACK_TO_START: return "Track to Start";
        case STATE_WAIT_CAR2_CMD:     return "Wait Car2 CMD";
		
        case STATE_STOP_INIT:         return "Stop Init";
        case STATE_ROTATE_CCW:        return "Rotate CCW";
        case STATE_PARKING_AREA:      return "Parking Area";
        case STATE_COUNT_PARK_FRAME:  return "Count Frame";
        case STATE_TURN_RIGHT:        return "Turn Right";
        case STATE_PARK_FINISH:       return "Park Finish";
        default:                      return "Unknown State";
    }
}

static void oled_display_entry(void* parameter)
{
    // 固定显示：第一行（第十三组）、第二行（Liu-JiaJun）
    OLED_ShowString(1, 1, "Team 13 Car - 1");        // 首行：第十三组的英文
    OLED_ShowString(2, 1, "Liu-JiaJun");     // 第二行：Liu-JiaJun
	OLED_ShowString(4, 1, "NULL");  // 显示第四行消息
    while (1)
    {
        // 第三行：实时显示状态机当前状态
        OLED_ShowString(3, 1, get_state_string(current_state));
        
        // 第四行：根据标志位更新发送/接收信息
        if (oled_update_flag == 1)
        {
            OLED_ShowString(4, 1, oled_msg_line4);  // 显示第四行消息
            oled_update_flag = 0;                   // 清除更新标志
        }
		
        rt_thread_mdelay(100);
    }
}






int main()
{
//    SystemInit();   
//	SysTick_Config(SystemCoreClock/1000);  /* 定时器1ms定时一次 */
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6) | (1UL << 8) | (1UL << 9) | (1UL << 10) | (1UL << 16); 
    LED_Init();
	UARTInit(115200);
	KEY_IRQ_Init();
	track_init();
	OLED_Init();       // 初始化OLED显示屏
    OLED_Clear();      // 清除OLED屏幕内容
	
	/* 静态初始化track_move线程 */
	rt_thread_init(
		&track_move_thread,             // 线程控制块
		"track_move",                   // 线程名称
		track_move_entry,               // 线程入口函数
		RT_NULL,                        // 入口参数
		&track_move_thread_stack[0],    // 线程堆栈起始地址
		sizeof(track_move_thread_stack),// 线程堆栈大小
		15,                             // 线程优先级（11-24之间）
		10                              // 时间片大小
	);


	/* 静态初始化OLED显示线程 */
	rt_thread_init(
		&oled_display_thread,           // 线程控制块
		"oled_display",                 // 线程名称
		oled_display_entry,             // 线程入口函数
		RT_NULL,                        // 入口参数
		&oled_display_thread_stack[0],  // 线程堆栈起始地址
		sizeof(oled_display_thread_stack),// 线程堆栈大小
		15,                             // 线程优先级（11-24之间）
		10                              // 时间片大小
	);


	/* 静态初始化通信线程 */
	rt_thread_init(
		&comm_thread,                   // 线程控制块
		"comm_thread",                  // 线程名称
		comm_thread_entry,              // 线程入口函数
		RT_NULL,                        // 入口参数
		&comm_thread_stack[0],          // 线程堆栈起始地址
		sizeof(comm_thread_stack),      // 线程堆栈大小
		15,                             // 线程优先级（11-24之间）
		10                              // 时间片大小
	);
		
	rt_thread_startup(&track_move_thread);  // 启动线程
	rt_thread_startup(&oled_display_thread);  // 启动线程
	rt_thread_startup(&comm_thread);  // 启动线程
		
//    OLED_ShowString(1, 4, "LiuJiajun ! ");    // 在第1行第4列显示字符串"LiuJiajun !"

}


