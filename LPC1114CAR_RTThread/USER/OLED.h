#ifndef __OLED_H
#define __OLED_H

#include "LPC11xx.h"


/******************************************************************************/
// OLED显示相关函数声明
/******************************************************************************/
void OLED_Init(void);  // 初始化OLED显示屏
void OLED_Clear(void);  // 清空OLED显示内容
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);  // 在指定行列显示一个字符
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);  // 在指定行列显示字符串
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示数字
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);  // 显示带符号的数字
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示16进制数
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);  // 显示二进制数

#endif
