#include "Motor.h"

// 芯片实际主频48MHz，定时器预分频后为1MHz（48MHz / 48，与timer16.h中MHZ_PRESCALE一致）
// PWM最佳频率选择10kHz（周期100us）
#define PWM_FREQ        10000UL    // 10kHz最佳频率
#define PWM_PERIOD      (1000000UL / PWM_FREQ)  // 1MHz计数频率下，周期计数值=1e6/1e4=100（单位：us）

/**
 * 初始化电机PWM：配置定时器16B0和16B1为10kHz PWM模式
 */
void motor_pwm_init(void) {
    // 初始化定时器16B0（右前轮：MAT0=反转，MAT1=正转）
    // 使能MAT0和MAT1通道（MATCH0 | MATCH1 = 0x03），关闭捕获功能
    init_timer16PWM(0, PWM_PERIOD, MATCH0 | MATCH1, 0);
    // 显式设置预分频器（48MHz / 48 = 1MHz计数频率，与timer16.h中MHZ_PRESCALE=48一致）
    LPC_TMR16B0->PR = MHZ_PRESCALE;
    
    // 初始化定时器16B1（右后轮：MAT0=反转，MAT1=正转）
    init_timer16PWM(1, PWM_PERIOD, MATCH0 | MATCH1, 0);
    LPC_TMR16B1->PR = MHZ_PRESCALE;
    
	// 初始化前轮32位定时器
    // 左前轮：CT32B1，MAT0=反转（PIO1-1），MAT1=正转（PIO1-2）
    init_timer32PWM(1, PWM_PERIOD, MATCH0 | MATCH1);  // 使能MAT0和MAT1通道
    LPC_TMR32B1->PR = MHZ_PRESCALE;  // 配置预分频为1MHz
    
    // 右前轮：CT32B0，MAT2=反转（PIO0-1），MAT3=正转（PIO0-11）
    init_timer32PWM(0, PWM_PERIOD, MATCH2 | MATCH3);  // 使能MAT2和MAT3通道
    LPC_TMR32B0->PR = MHZ_PRESCALE;  // 配置预分频为1MHz
    
	
    // 启动定时器
    enable_timer16(0);
    enable_timer16(1);
    // 启动32位定时器
    enable_timer32(0);
    enable_timer32(1);

	right_rear_motor(MOTOR_STOP, 0);
	left_rear_motor(MOTOR_STOP, 0);
	left_front_motor(MOTOR_STOP , 0);
	right_front_motor(MOTOR_STOP , 0);
}

/**
 * 右后轮控制逻辑（CT16B1，10kHz PWM）
 * direction: 方向（正转/反转/停止）
 * duty_cycle: 占空比（0-100）
 */
void right_rear_motor(MotorDirection direction, uint8_t duty_cycle) {
    uint32_t pwm_value;
    
    // 限制占空比范围（0-100）
    if (duty_cycle > 100) duty_cycle = 100;
    
    // 计算PWM匹配值（占空比 = 匹配值 / PWM_PERIOD）
    // 10kHz下PWM_PERIOD=100，故pwm_value = (100 * 占空比)/100 = 占空比（简化计算）
    pwm_value = (PWM_PERIOD * duty_cycle) / 100;
    
    switch (direction) {
        case MOTOR_FORWARD:
            // 正转：MAT0（PIO1-9）输出PWM，MAT1（PIO1-10）关闭
            setMatch_timer16PWM(1, 0, pwm_value);
            setMatch_timer16PWM(1, 1, 0);
            break;
            
        case MOTOR_BACKWARD:

			// 反转：MAT1（PIO1-10）输出PWM，MAT0（PIO1-9）关闭
            setMatch_timer16PWM(1, 1, pwm_value);
            setMatch_timer16PWM(1, 0, 0);
            break;
            
        case MOTOR_STOP:
        default:
            // 停止：双通道均输出0
            setMatch_timer16PWM(1, 0, 0);
            setMatch_timer16PWM(1, 1, 0);
            break;
    }
}

/**
 * 左后轮控制逻辑（CT16B0，10kHz PWM）
 * 参数同后轮
 */
void left_rear_motor(MotorDirection direction, uint8_t duty_cycle) {
    uint32_t pwm_value;
    
    if (duty_cycle > 100) duty_cycle = 100;
    pwm_value = (PWM_PERIOD * duty_cycle) / 100;  // 同后轮计算逻辑
    
    switch (direction) {
        case MOTOR_FORWARD:
            // 正转：MAT1（PIO0-9）输出PWM，MAT0（PIO0-8）关闭
            setMatch_timer16PWM(0, 1, pwm_value);
            setMatch_timer16PWM(0, 0, 0);
            break;
            
        case MOTOR_BACKWARD:
            // 反转：MAT0（PIO0-8）输出PWM，MAT1（PIO0-9）关闭
            setMatch_timer16PWM(0, 0, pwm_value);
            setMatch_timer16PWM(0, 1, 0);
            break;
            
        case MOTOR_STOP:
        default:
            // 停止：双通道均输出0
            setMatch_timer16PWM(0, 0, 0);
            setMatch_timer16PWM(0, 1, 0);
            break;
    }
}

/**
 * 左前轮控制逻辑（CT32B1，10kHz PWM）
 * direction: 方向（正转/反转/停止）
 * duty_cycle: 占空比（0-100）
 */
void left_front_motor(MotorDirection direction, uint8_t duty_cycle) {
    uint32_t pwm_value;
    
    if (duty_cycle > 100) duty_cycle = 100;
    pwm_value = (PWM_PERIOD * duty_cycle) / 100;  // 同后轮占空比计算逻辑
    
    switch (direction) {
        case MOTOR_FORWARD:
            // 正转：MAT1（PIO1-2）输出PWM，MAT0（PIO1-1）关闭
            setMatch_timer32PWM(1, 1, pwm_value);
            setMatch_timer32PWM(1, 0, 0);
            break;
            
        case MOTOR_BACKWARD:
            // 反转：MAT0（PIO1-1）输出PWM，MAT1（PIO1-2）关闭
            setMatch_timer32PWM(1, 0, pwm_value);
            setMatch_timer32PWM(1, 1, 0);
            break;
            
        case MOTOR_STOP:
        default:
            // 停止：双通道均输出0
            setMatch_timer32PWM(1, 0, 0);
            setMatch_timer32PWM(1, 1, 0);
            break;
    }
}

/**
 * 右前轮控制逻辑（CT32B0，10kHz PWM）
 * direction: 方向（正转/反转/停止）
 * duty_cycle: 占空比（0-100）
 */
void right_front_motor(MotorDirection direction, uint8_t duty_cycle) {
    uint32_t pwm_value;
    
    if (duty_cycle > 100) duty_cycle = 100;
    pwm_value = (PWM_PERIOD * duty_cycle) / 100;  // 同后轮占空比计算逻辑
    
    switch (direction) {
        case MOTOR_FORWARD:

		    // 正转：MAT2（PIO0-1）输出PWM，MAT3（PIO0-11）关闭
            setMatch_timer32PWM(0, 2, pwm_value);
            setMatch_timer32PWM(0, 3, 0);
            break;
            
        case MOTOR_BACKWARD:
            // 反转：MAT3（PIO0-11）输出PWM，MAT2（PIO0-1）关闭
            setMatch_timer32PWM(0, 3, pwm_value);
            setMatch_timer32PWM(0, 2, 0);
            break;
            
        case MOTOR_STOP:
        default:
            // 停止：双通道均输出0
            setMatch_timer32PWM(0, 2, 0);
            setMatch_timer32PWM(0, 3, 0);
            break;
    }
}