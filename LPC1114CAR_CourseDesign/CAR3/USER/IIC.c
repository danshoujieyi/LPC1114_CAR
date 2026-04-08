#include "IIC.h"

void IIC_Init(uint8_t Mode)
{
  LPC_SYSCON->PRESETCTRL |= (1<<1);   //使I2C上电 I2C模块（在启动I2C模块之前，必须向该位写1）
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);   //使能I2C时钟
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);   // 使能IOCON时钟
  LPC_IOCON->PIO0_4 &= ~0x3F;  
  LPC_IOCON->PIO0_4 |= 0x01;   //把P0.4脚配置为 I2C SCL 
  LPC_IOCON->PIO0_5 &= ~0x3F;  
  LPC_IOCON->PIO0_5 |= 0x01;   //把P0.5脚配置为 I2C SDA 
	
	 /*--- Reset registers ---*/
	if(Mode == 1)
	{
	 LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
	 LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
	}
	else
	{
    LPC_I2C->SCLH = I2SCLH_SCLH;
    LPC_I2C->SCLL = I2SCLL_SCLL;
	}
 /*--- Clear flags ---*/
  LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;      
	   
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);   // 禁能IOCON时钟 
  LPC_I2C->CONSET |= I2CONSET_I2EN;  // 使能I2C接口
}


void IIC_Start(void)
{
  LPC_I2C->CONSET |= I2CONSET_STA;  // 使能发送开始信号
  while(!(LPC_I2C->CONSET & I2CONSET_SI));  // 等待开始信号发送完成
  LPC_I2C->CONCLR = I2CONCLR_STAC | I2CONCLR_SIC; // 清除开始START位和SI位 
}


void IIC_Stop(void)
{
   LPC_I2C->CONCLR = I2CONCLR_SIC;  // 清SI标志位
   LPC_I2C->CONSET |= I2CONSET_STO;  // 发送停止信号
   while( LPC_I2C->CONSET & I2CONSET_STO );  // 等待停止信号发送完成 
}

/****************************************************/
/* 函数功能：I2C发送一字节数据  */
/* 入口参数：dat : 要发送的字节  */
/***************************************************/
void IIC_Send_Byte(uint8_t dat)
{
   LPC_I2C->DAT = dat;  // 把字节写入DAT寄存器
   LPC_I2C->CONCLR = I2CONSET_SI;  // 清除SI标志(1<<3)
   while(!(LPC_I2C->CONSET & I2CONSET_SI));  // 等待数据发送完成
}


/***************************************************/
/* 函数功能：I2C接收一字节数据  */
/* 入口参数：rebyte : 要接收的字节  */
/***************************************************/
uint8_t IIC_Recieve_Byte(void)
{
  uint8_t ReByte;
  LPC_I2C->CONCLR = I2CONCLR_SIC;   // 清AA和SI标志
  while(!(LPC_I2C->CONSET & I2CONSET_SI));  // 等待接收数据完成
  ReByte = (uint8_t)LPC_I2C->DAT;  // 把接收到的数据给了rebyte
  return ReByte;
}
