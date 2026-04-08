#include "OLED.h"
#include "OLED_Font.h"
#include "Delay.h"
#include "driver_config.h"
#include "i2c.h"

// 外部引用I2C全局变量（来自i2c.c）
extern volatile uint8_t I2CMasterBuffer[I2CBUFSIZE];
extern volatile uint32_t I2CWriteLength;
extern volatile uint32_t I2CReadLength;
extern volatile uint32_t I2CMasterState;


/**
 * 函数功能：显示当前信息到OLED屏幕
 * 描述：在OLED屏幕上依次显示当前日期、时间和温度。日期、时间和温度的数据通过全局变量传递。
 * OLED显示的位置和数据格式固定，显示日期、时间和温度值。
 */
void Menu(void)
{   
    /*OLED显示*/
    // OLED_Clear(); // 清除OLED显示屏（注释掉了此行）
    OLED_ShowString(1, 4, "LiuJiajun ! ");    // 在第1行第4列显示字符串"LiuJiajun !"
    OLED_ShowString(2, 1, "DATE: ");          // 在第2行第1列显示"DATE: "
    // // 显示日期（Date[]数组保存了日期数据）
    // OLED_ShowChar(2, 7, Date[0]);
    // OLED_ShowChar(2, 8, Date[1]);
    // OLED_ShowChar(2, 9, Date[2]);
    // OLED_ShowChar(2, 10, Date[3]);
    // OLED_ShowChar(2, 11, Date[4]);
    // OLED_ShowChar(2, 12, Date[5]);
    // OLED_ShowChar(2, 13, Date[6]);
    // OLED_ShowChar(2, 14, Date[7]);
    // OLED_ShowChar(2, 15, Date[8]);
    // OLED_ShowChar(2, 16, Date[9]); 
    
    // OLED_ShowString(3, 1, "TIME:  ");        // 在第3行第1列显示"TIME: "
    // // 显示时间（Time_Data[]数组保存了时间数据）
    // OLED_ShowChar(3, 8, Time_Data[0]);
    // OLED_ShowChar(3, 9, Time_Data[1]);
    // OLED_ShowChar(3, 10, Time_Data[2]);
    // OLED_ShowChar(3, 11, Time_Data[3]);
    // OLED_ShowChar(3, 12, Time_Data[4]);
    // OLED_ShowChar(3, 13, Time_Data[5]);
    // OLED_ShowChar(3, 14, Time_Data[6]);
    // OLED_ShowChar(3, 15, Time_Data[7]);
    
    // OLED_ShowString(4, 1, "Temp:  ");        // 在第4行第1列显示"Temp: "
    // // 显示温度（Temp_Buf[]数组保存了温度数据）
    // OLED_ShowChar(4, 8, Temp_Buf[0]);
    // OLED_ShowChar(4, 9, Temp_Buf[1]);
    // OLED_ShowChar(4, 10, Temp_Buf[2]);
    // OLED_ShowChar(4, 11, Temp_Buf[3]);
    // OLED_ShowChar(4, 12, Temp_Buf[4]);
    // OLED_ShowChar(4, 13, Temp_Buf[5]);
    // OLED_ShowString(4, 15, "C");             // 在温度后显示"℃"
}



void OLED_WriteCommand(uint8_t Command)
{
    // 填充发送缓冲区：从机地址(0x78) + 命令标志(0x00) + 命令
    I2CMasterBuffer[0] = 0x78;  // OLED的I2C写地址（7位地址0x3C左移1位+0）
    I2CMasterBuffer[1] = 0x00;  // 写命令标志
    I2CMasterBuffer[2] = Command;
    
    // 配置发送参数：写3个字节，不读数据
    I2CWriteLength = 3;
    I2CReadLength = 0;
    
    // 启动I2C事务
    I2CMasterState = I2C_BUSY;
    I2CEngine();
    
    // 等待事务完成（简单处理，实际可加超时判断）
    while (I2CMasterState == I2C_BUSY);
}

void OLED_WriteData(uint8_t Data)
{
    // 填充发送缓冲区：从机地址(0x78) + 数据标志(0x40) + 数据
    I2CMasterBuffer[0] = 0x78;  // OLED的I2C写地址
    I2CMasterBuffer[1] = 0x40;  // 写数据标志
    I2CMasterBuffer[2] = Data;
    
    // 配置发送参数：写3个字节，不读数据
    I2CWriteLength = 3;
    I2CReadLength = 0;
    
    // 启动I2C事务
    I2CMasterState = I2C_BUSY;
    I2CEngine();
    
    // 等待事务完成
    while (I2CMasterState == I2C_BUSY);
}


/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	I2CInit(I2CMASTER);  // 初始化I2C为主机模式（自动配置P0.4=SCL，P0.5=SDA）
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
