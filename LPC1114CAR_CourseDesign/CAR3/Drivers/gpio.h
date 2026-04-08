/*****************************************************************************
 *   $Id:: gpio.h 4790 2010-09-03 23:35:38Z nxp21346                        $
 *   项目：NXP LPC11xx软件示例
 *
 *   描述：
 *     此文件包含GPIO的定义和原型。
 *
 ****************************************************************************
 * 此处描述的软件仅用于说明目的，为客户提供有关产品的编程信息。
 * 本软件按"现状"提供，不附带任何担保。NXP半导体公司对本软件的使用不承担任何责任或义务，
 * 不对本产品的任何专利、版权或掩模作品权利授予任何许可或所有权。
 * NXP半导体公司保留在不通知的情况下对软件进行修改的权利。
 * NXP半导体公司也不保证此类应用在未经进一步测试或修改的情况下适用于特定用途。
****************************************************************************/
#ifndef __GPIO_H 
#define __GPIO_H
#if CONFIG_ENABLE_DRIVER_GPIO==1

#define PORT0		0
#define PORT1		1
#define PORT2		2
#define PORT3		3
static LPC_GPIO_TypeDef (* const LPC_GPIO[4]) = { LPC_GPIO0, LPC_GPIO1, LPC_GPIO2, LPC_GPIO3 };
void PIOINT0_IRQHandler(void);
void PIOINT1_IRQHandler(void);
void PIOINT2_IRQHandler(void);
void PIOINT3_IRQHandler(void);
void GPIOInit( void );
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
		uint32_t single, uint32_t event );

void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi );
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi );
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi );
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi );
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal );
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir );
uint8_t GPIOGetValue(uint32_t portNum, uint32_t bitPosi);
#endif
#endif /* end __GPIO_H */
/*****************************************************************************
**                            文件结束
******************************************************************************/