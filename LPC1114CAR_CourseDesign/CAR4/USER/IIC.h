#ifndef __IIC_H
#define __IIC_H

#include "LPC11xx.h"

/******************************************************************************/
/* I2C 控制寄存器 */  
#define I2CONSET_AA			0x00000004  // 是否产生应答信号允许位，即是否设置为从机模式
#define I2CONSET_SI			0x00000008  // I2C中断标志位
#define I2CONSET_STO		0x00000010 // 停止标志位
#define I2CONSET_STA		0x00000020  // 开始标志位
#define I2CONSET_I2EN		0x00000040   // I2C接口允许位
/* I2C “清控制寄存器位”寄存器 */
#define I2CONCLR_AAC		0x00000004    // 清应答信号允许位
#define I2CONCLR_SIC		0x00000008  // 清I2C中断标志位
#define I2CONCLR_STAC		0x00000020  // 清开始标志位
#define I2CONCLR_I2ENC		0x00000040  // 清I2C接口允许位
/* I2C 通信速率宏定义 */ 
#define I2SCLH_SCLH			0x00000180  /* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL			0x00000180  /* I2C SCL Duty Cycle Low Reg */
#define I2SCLH_HS_SCLH		0x00000030  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL		0x00000030  /* Fast Plus I2C SCL Duty Cycle Low Reg */

#define TRUE 1
#define FALSE 0
 
/******************************************************************************/
// I2C总线通信相关函数声明
/******************************************************************************/
void IIC_Init(uint8_t Mode);  // 初始化I2C通信，选择模式（标准/快速）
void IIC_Start(void);  // 启动I2C通信
void IIC_Stop(void);  // 停止I2C通信
void IIC_Send_Byte(uint8_t dat);  // 发送一个字节数据
uint8_t IIC_Recieve_Byte(void);  // 接收一个字节数据


#endif
