#include "LED.h"
#include "Delay.h"
#include "rtthread.h"

/******************************************************************************/
//void LED_Init(void)
//{
//   LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 6); /* GPIO 时钟使能 */
//   LPC_GPIO3->DIR |= (1UL << 3); /*将 GPIO1_9 作为输出口*/
//}


//void LED_Toggle(void)
//{
//	LPC_GPIO3->DATA ^= (1UL <<3);/* LED 闪烁切换 */
//	T16B0_delay_ms(1000);
//}



// LED初始化函数：配置LED对应的GPIO引脚为输出
void LED_Init(void)
{
    GPIOInit();  // 调用GPIO初始化函数（已包含GPIO时钟使能，无需重复操作）
    // 使用GPIOSetDir设置GPIO3的第3位为输出（替代直接操作DIR寄存器）
    // 参数：端口号PORT3，引脚3，方向1（输出）
    GPIOSetDir(PORT3, 3, 1);
	
	    // 右后灯：PIO0_7设置为输出
    GPIOSetDir(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 1);
    // 左后灯：PIO0_6设置为输出
    GPIOSetDir(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 1);
    // 右前灯：PIO2_5设置为输出
    GPIOSetDir(RIGHT_FRONT_LIGHT_PORT, RIGHT_FRONT_LIGHT_PIN, 1);
    // 左前灯：PIO2_4设置为输出
    GPIOSetDir(LEFT_FRONT_LIGHT_PORT, LEFT_FRONT_LIGHT_PIN, 1);
    
    // 初始状态：所有灯关闭（默认输出低电平）
    RightRearLight_Off();
    LeftRearLight_Off();
    RightFrontLight_Off();
    LeftFrontLight_Off();
}

// LED电平翻转函数：切换LED对应的GPIO引脚电平
void LED_Toggle(void)
{
    // 读取当前GPIO3_3的电平（通过寄存器直接读取，因驱动未提供读函数）
    uint32_t current_val = (LPC_GPIO3->DATA & (1UL << 3)) ? 1 : 0;
    // 翻转电平：若当前为高则设为低，反之亦然（使用GPIOSetValue封装函数）
    GPIOSetValue(PORT3, 3, !current_val);
    rt_thread_mdelay(1000);  // 保留原有延时（假设该函数已实现）
}

void LED_On(void)
{
    GPIOSetValue(PORT3, 3, 0);
}

void LED_Off(void)
{
    GPIOSetValue(PORT3, 3, 1);
}
/**
 * @brief 打开右后灯（输出高电平）
 */
void RightRearLight_On(void)
{
    GPIOSetValue(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 1);
}

/**
 * @brief 关闭右后灯（输出低电平）
 */
void RightRearLight_Off(void)
{
    GPIOSetValue(RIGHT_REAR_LIGHT_PORT, RIGHT_REAR_LIGHT_PIN, 0);
}

/**
 * @brief 打开左后灯（输出高电平）
 */
void LeftRearLight_On(void)
{
    GPIOSetValue(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 1);
}

/**
 * @brief 关闭左后灯（输出低电平）
 */
void LeftRearLight_Off(void)
{
    GPIOSetValue(LEFT_REAR_LIGHT_PORT, LEFT_REAR_LIGHT_PIN, 0);
}

/**
 * @brief 打开右前灯（输出高电平）
 */
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