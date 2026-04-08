/*
 * driver_config.h
 *
 *  Created on: Aug 31, 2010
 *      Author: nxp28548
 */

#ifndef DRIVER_CONFIG_H_
#define DRIVER_CONFIG_H_

#include <LPC11xx.h>

#define CONFIG_ENABLE_DRIVER_CRP						1
#define CONFIG_CRP_SETTING_NO_CRP						1

#define CONFIG_ENABLE_DRIVER_TIMER32					1
#define CONFIG_TIMER32_DEFAULT_TIMER32_0_IRQHANDLER		0

#define CONFIG_ENABLE_DRIVER_TIMER16					1
#define CONFIG_TIMER16_DEFAULT_TIMER16_0_IRQHANDLER		0

#define CONFIG_ENABLE_DRIVER_GPIO						1
#define CONFIG_GPIO_DEFAULT_PIOINT0_IRQHANDLER          0

#define CONFIG_ENABLE_DRIVER_SSP                        0

#define CONFIG_ENABLE_DRIVER_I2C                        1
#define CONFIG_I2C_DEFAULT_I2C_IRQHANDLER               1

#define CONFIG_ENABLE_DRIVER_UART        1  // 使能UART驱动
//#define CONFIG_UART_DEFAULT_UART_IRQHANDLER 1  // 使用驱动的中断服务程序
//#define CONFIG_UART_ENABLE_INTERRUPT     1  // 使能UART中断
//#define CONFIG_UART_ENABLE_TX_INTERRUPT  0  // 0=轮询发送，1=中断发送（根据需求选择）

#endif
