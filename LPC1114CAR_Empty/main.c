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
#include "stdio.h"
#include "OLED.h"
#include "uart.h"
#include "i2c.h"


int main()
{
    SystemInit();   
	SysTick_Config(SystemCoreClock/1000);  /* sytick定时器1ms定时一次 */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6) | (1UL << 8) | (1UL << 9) | (1UL << 10) | (1UL << 16); 
    LED_Init();
	UARTInit(115200);
	OLED_Init();       
    OLED_Clear();     
    OLED_ShowString(1, 7, "HNU ! ");    
	OLED_ShowString(2, 2, "danshoujieyi ! ");   
	motor_pwm_init();


	while (1)                               
	{
		
		right_rear_motor(MOTOR_FORWARD, 20);
		left_rear_motor(MOTOR_FORWARD, 20);
		left_front_motor(MOTOR_FORWARD , 20);
		right_front_motor(MOTOR_FORWARD , 20);
         
		printf("HNU-danshoujieyi \r\n");
		delay_ms(10);
		LED_On();
		delay_ms(500);
		LED_Off();
		delay_ms(500);
		RightRearLight_On();    // 右后灯打开
		delay_ms(500);
		RightRearLight_Off();   // 右后灯关闭
		delay_ms(500);
		LeftRearLight_On();     // 左后灯打开
		delay_ms(500);
		LeftRearLight_Off();    // 左后灯关闭
		delay_ms(500);
		RightFrontLight_On();   // 右前灯打开
		delay_ms(500);
		RightFrontLight_Off();  // 右前灯关闭
		delay_ms(500);
		LeftFrontLight_On();    // 左前灯打开
		delay_ms(500);
		LeftFrontLight_Off();   // 左前灯关闭
		delay_ms(500);

	}

}


