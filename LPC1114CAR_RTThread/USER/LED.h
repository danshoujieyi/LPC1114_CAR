#ifndef __LED_H
#define __LED_H

#include "LPC11xx.h"
#include "driver_config.h"
#include "gpio.h"  // 包含GPIO驱动头文件


// LED引脚定义（对应硬件连接）
#define RIGHT_REAR_LIGHT_PORT  PORT0   // 右后灯：PIO0_7
#define RIGHT_REAR_LIGHT_PIN   7
#define LEFT_REAR_LIGHT_PORT   PORT0    // 左后灯：PIO0_6
#define LEFT_REAR_LIGHT_PIN    6
#define RIGHT_FRONT_LIGHT_PORT PORT2   // 右前灯：PIO2_5
#define RIGHT_FRONT_LIGHT_PIN  5
#define LEFT_FRONT_LIGHT_PORT  PORT2    // 左前灯：PIO2_4
#define LEFT_FRONT_LIGHT_PIN   4

// 初始化所有LED引脚（设置为输出）
void LED_Init(void);
void LED_On(void);
void LED_Off(void);

// 右后灯控制
void RightRearLight_On(void);    // 右后灯打开
void RightRearLight_Off(void);   // 右后灯关闭

// 左后灯控制
void LeftRearLight_On(void);     // 左后灯打开
void LeftRearLight_Off(void);    // 左后灯关闭

// 右前灯控制
void RightFrontLight_On(void);   // 右前灯打开
void RightFrontLight_Off(void);  // 右前灯关闭

// 左前灯控制
void LeftFrontLight_On(void);    // 左前灯打开
void LeftFrontLight_Off(void);   // 左前灯关闭


#endif

