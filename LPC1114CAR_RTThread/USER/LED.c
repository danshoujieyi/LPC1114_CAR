#include "LED.h"
#include "rtthread.h"


// LED初始化函数
void LED_Init(void)
{
    GPIOInit();  
    GPIOSetDir(PORT3, 3, 1);
	
	    // 右后灯：PIO0_7设置为输出
    GPIOSetDir(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 1);
    // 左后灯：PIO0_6设置为输出
    GPIOSetDir(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 1);
    // 右前灯：PIO2_5设置为输出
    GPIOSetDir(RIGHT_FRONT_LIGHT_PORT, RIGHT_FRONT_LIGHT_PIN, 1);
    // 左前灯：PIO2_4设置为输出
    GPIOSetDir(LEFT_FRONT_LIGHT_PORT, LEFT_FRONT_LIGHT_PIN, 1);
    
    // 初始状态：所有灯关闭
	LED_Off();
    RightRearLight_Off();
    LeftRearLight_Off();
    RightFrontLight_Off();
    LeftFrontLight_Off();
}


void LED_On(void)
{
    GPIOSetValue(PORT3, 3, 0);
}

void LED_Off(void)
{
    GPIOSetValue(PORT3, 3, 1);
}

void RightRearLight_On(void)
{
    GPIOSetValue(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 1);
}


void RightRearLight_Off(void)
{
    GPIOSetValue(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 0);
}


void LeftRearLight_On(void)
{
    GPIOSetValue(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 1);
}


void LeftRearLight_Off(void)
{
    GPIOSetValue(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 0);
}


void RightFrontLight_On(void)
{
    GPIOSetValue(RIGHT_FRONT_LIGHT_PORT, RIGHT_FRONT_LIGHT_PIN, 1);
}

/**
 * @brief 关闭右前灯（输出低电平）
 */
void RightFrontLight_Off(void)
{
    GPIOSetValue(RIGHT_FRONT_LIGHT_PORT, RIGHT_FRONT_LIGHT_PIN, 0);
}

/**
 * @brief 打开左前灯（输出高电平）
 */
void LeftFrontLight_On(void)
{
    GPIOSetValue(LEFT_FRONT_LIGHT_PORT, LEFT_FRONT_LIGHT_PIN, 1);
}

/**
 * @brief 关闭左前灯（输出低电平）
 */
void LeftFrontLight_Off(void)
{
    GPIOSetValue(LEFT_FRONT_LIGHT_PORT, LEFT_FRONT_LIGHT_PIN, 0);
}


/******************************************************************************/