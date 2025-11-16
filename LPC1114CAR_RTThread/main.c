#include "LPC11xx.h"
#include "driver_config.h"
#include "target_config.h"
#include "timer32.h"
#include "LPC11xx.h"
#include "driver_config.h"
#include "target_config.h"
#include "timer32.h"
#include "timer16.h"
#include "gpio.h"
#include "LED.h"
#include "Motor.h"
#include "ssp.h"
#include "stdio.h"
#include "OLED.h"
#include "uart.h"
#include "i2c.h"
#include "Key.h"
#include "rtthread.h"



extern volatile uint8_t key_flag;

/* 定义线程控制块和栈空间 */
static struct rt_thread led_thread;
static struct rt_thread print_thread;
static rt_uint8_t led_thread_stack[1024];
static rt_uint8_t print_thread_stack[1024];


/* LED闪烁线程入口函数 */
static void led_thread_entry(void* parameter)
{

    while (1)
    {
		RightRearLight_On();    // 右后灯打开
		rt_thread_mdelay(500);
		RightRearLight_Off();   // 右后灯关闭
		rt_thread_mdelay(500);
		LeftRearLight_On();     // 左后灯打开
		rt_thread_mdelay(500);
		LeftRearLight_Off();    // 左后灯关闭
		rt_thread_mdelay(500);
		RightFrontLight_On();   // 右前灯打开
		rt_thread_mdelay(500);
		RightFrontLight_Off();  // 右前灯关闭
		rt_thread_mdelay(500);
		LeftFrontLight_On();    // 左前灯打开
		rt_thread_mdelay(500);
		LeftFrontLight_Off();   // 左前灯关闭
		rt_thread_mdelay(500);
    }
}

/* 打印线程入口函数 */
static void print_thread_entry(void* parameter)
{
    while (1)
    {
		if(key_flag == 1)
		{
			key_flag = 0;
			LED_On();
		}
		right_rear_motor(MOTOR_FORWARD, 20);
		left_rear_motor(MOTOR_FORWARD, 20);
		left_front_motor(MOTOR_FORWARD , 20);
		right_front_motor(MOTOR_FORWARD , 20);
		
		printf("HNU-danshoujieyi \r\n");
        rt_thread_mdelay(100);
    }
}



int main()
{
	// 下面三个函数已经在RT-Thread中初始化
//    SystemInit();   
//	SysTick_Config(SystemCoreClock/1000);  
//	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6) | (1UL << 8) | (1UL << 9) | (1UL << 10) | (1UL << 16); 
    LED_Init();
	UARTInit(115200);
	OLED_Init();       
    OLED_Clear();     
    OLED_ShowString(1, 7, "HNU ! ");    
	OLED_ShowString(2, 2, "danshoujieyi ! ");   
	motor_pwm_init();
	KEY_IRQ_Init();
	
	    /* 静态初始化LED闪烁线程 */
    rt_thread_init(
        &led_thread,                 // 线程控制块
        "led_thread",                // 线程名称
        led_thread_entry,            // 线程入口函数
        RT_NULL,                     // 入口参数
        &led_thread_stack[0],        // 线程堆栈起始地址
        sizeof(led_thread_stack),    // 线程堆栈大小
        15,                          // 线程优先级(11-24之间),// 优先级请设置高于25，因为tshell交互线程优先级为25
        10                           // 时间片大小
		// // 优先级，数值越大优先级越低，优先级必须设置在11-24之间，因为main线程优先级为10.优先级通常取15
                      // 使用时间片轮询，单个线程时间只允许运行不超过10TICKS
    );
    rt_thread_startup(&led_thread);    // 启动线程
    
    /* 静态初始化打印线程 */
    rt_thread_init(
        &print_thread,               // 线程控制块
        "print_thread",              // 线程名称
        print_thread_entry,          // 线程入口函数
        RT_NULL,                     // 入口参数
        &print_thread_stack[0],      // 线程堆栈起始地址
        sizeof(print_thread_stack),  // 线程堆栈大小
        15,                          // 线程优先级(与LED线程相同)
        10                           // 时间片大小
    );
    rt_thread_startup(&print_thread);  // 启动线程


}


