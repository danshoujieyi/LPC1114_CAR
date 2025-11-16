/****************************************************************************
 *   $Id:: gpio.c 4785 2010-09-03 22:39:27Z nxp21346                        $
 *   项目: NXP LPC11xx GPIO示例
 *
 *   描述:
 *     此文件包含GPIO代码示例，包括GPIO的初始化、GPIO中断处理程序以及相关的GPIO访问API。
 *
 ****************************************************************************
 * 本文所述软件仅用于说明目的，为客户提供有关产品的编程信息。
 * 本软件按"原样"提供，不附带任何保证。恩智浦半导体不对软件的使用承担任何责任或义务，
 * 不对产品的任何专利、版权或掩模作品权授予许可或所有权。恩智浦半导体保留在不通知的情况下修改软件的权利。
 * 恩智浦半导体也不保证此类应用在未经进一步测试或修改的情况下适用于特定用途。
****************************************************************************/
#include "driver_config.h"
#if CONFIG_ENABLE_DRIVER_GPIO==1
#include "gpio.h"

#if CONFIG_GPIO_DEFAULT_PIOINT0_IRQHANDLER==1
volatile uint32_t gpio0_counter = 0;
volatile uint32_t p0_1_counter  = 0;
/*****************************************************************************
** 函数名称:		PIOINT0_IRQHandler
**
** 功能描述:		使用一个GPIO引脚（端口0引脚1）作为中断源
**
** 参数:			无
** 返回值:		无
** 
*****************************************************************************/
void PIOINT0_IRQHandler(void)
{
  uint32_t regVal;

  gpio0_counter++;
  regVal = GPIOIntStatus( PORT0, 1 );
  if ( regVal )
  {
	p0_1_counter++;
	GPIOIntClear( PORT0, 1 );
  }		
  return;
}
#endif

#if CONFIG_GPIO_DEFAULT_PIOINT1_IRQHANDLER==1
volatile uint32_t gpio1_counter = 0;
volatile uint32_t p1_1_counter  = 0;
/*****************************************************************************
** 函数名称:		PIOINT1_IRQHandler
**
** 功能描述:		使用一个GPIO引脚（端口1引脚1）作为中断源
**
** 参数:			无
** 返回值:		无
** 
*****************************************************************************/
void PIOINT1_IRQHandler(void)
{
  uint32_t regVal;

  gpio1_counter++;
  regVal = GPIOIntStatus( PORT1, 1 );
  if ( regVal )
  {
	p1_1_counter++;
	GPIOIntClear( PORT1, 1 );
  }		
  return;
}
#endif

#if CONFIG_GPIO_DEFAULT_PIOINT2_IRQHANDLER==1
volatile uint32_t gpio2_counter = 0;
volatile uint32_t p2_1_counter  = 0;
/*****************************************************************************
** 函数名称:		PIOINT2_IRQHandler
**
** 功能描述:		使用一个GPIO引脚（端口2引脚1）作为中断源
**
** 参数:			无
** 返回值:		无
** 
*****************************************************************************/
void PIOINT2_IRQHandler(void)
{
  uint32_t regVal;

  gpio2_counter++;
  regVal = GPIOIntStatus( PORT2, 1 );
  if ( regVal )
  {
	p2_1_counter++;
	GPIOIntClear( PORT2, 1 );
  }		
  return;
}
#endif

#if CONFIG_GPIO_DEFAULT_PIOINT3_IRQHANDLER==1
volatile uint32_t gpio3_counter = 0;
volatile uint32_t p3_1_counter  = 0;
/*****************************************************************************
** 函数名称:		PIOINT3_IRQHandler
**
** 功能描述:		使用一个GPIO引脚（端口3引脚1）作为中断源
**
** 参数:			无
** 返回值:		无
** 
*****************************************************************************/
void PIOINT3_IRQHandler(void)
{
  uint32_t regVal;

  gpio3_counter++;
  regVal = GPIOIntStatus( PORT3, 1 );
  if ( regVal )
  {
	p3_1_counter++;
	GPIOIntClear( PORT3, 1 );
  }		
  return;
}
#endif //#if CONFIG_GPIO_DEFAULT_PIOINT3_IRQHANDLER==1


/*****************************************************************************
** 函数名称:		GPIOInit
**
** 功能描述:		初始化GPIO，安装GPIO中断处理程序
**
** 参数:			无
** 返回值:		布尔值（true或false），如果VIC表已满且无法安装GPIO中断处理程序，则返回false
** 
*****************************************************************************/
void GPIOInit( void )
{
  /* 使能GPIO域的AHB时钟。 */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

#ifdef __JTAG_DISABLED  
  LPC_IOCON->R_PIO1_1  &= ~0x07;
  LPC_IOCON->R_PIO1_1  |= 0x01;
#endif

  /* 当I/O引脚配置为外部中断时，设置NVIC。 */
#if CONFIG_GPIO_DEFAULT_PIOINT0_IRQHANDLER==1
  NVIC_EnableIRQ(EINT0_IRQn);
#endif
#if CONFIG_GPIO_DEFAULT_PIOINT1_IRQHANDLER==1
  NVIC_EnableIRQ(EINT1_IRQn);
#endif
#if CONFIG_GPIO_DEFAULT_PIOINT2_IRQHANDLER==1
  NVIC_EnableIRQ(EINT2_IRQn);
#endif
#if CONFIG_GPIO_DEFAULT_PIOINT3_IRQHANDLER==1
  NVIC_EnableIRQ(EINT3_IRQn);
#endif
  return;
}

/*****************************************************************************
** 函数名称:		GPIOSetInterrupt
**
** 功能描述:		设置中断的触发方式（电平或边沿）、事件（单沿或双沿）等
**						边沿或电平，0为边沿，1为电平
**						单沿或双沿，0为单沿，1为双沿
**						高电平有效或低电平有效等
**
** 参数:			端口号、位位置、触发方式（电平/边沿）、单沿/双沿、极性
** 返回值:		无
** 
*****************************************************************************/
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
			uint32_t single, uint32_t event )
{
  switch ( portNum )
  {
	case PORT0:
	  if ( sense == 0 )
	  {
		LPC_GPIO0->IS &= ~(0x1<<bitPosi);
		/* 仅当触发方式为0（边沿触发）时，单沿/双沿才有效。 */
		if ( single == 0 )
		  LPC_GPIO0->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO0->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO0->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO0->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO0->IEV |= (0x1<<bitPosi);
	break;
 	case PORT1:
	  if ( sense == 0 )
	  {
		LPC_GPIO1->IS &= ~(0x1<<bitPosi);
		/* 仅当触发方式为0（边沿触发）时，单沿/双沿才有效。 */
		if ( single == 0 )
		  LPC_GPIO1->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO1->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO1->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO1->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO1->IEV |= (0x1<<bitPosi);  
	break;
	case PORT2:
	  if ( sense == 0 )
	  {
		LPC_GPIO2->IS &= ~(0x1<<bitPosi);
		/* 仅当触发方式为0（边沿触发）时，单沿/双沿才有效。 */
		if ( single == 0 )
		  LPC_GPIO2->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO2->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO2->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO2->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO2->IEV |= (0x1<<bitPosi);  
	break;
	case PORT3:
	  if ( sense == 0 )
	  {
		LPC_GPIO3->IS &= ~(0x1<<bitPosi);
		/* 仅当触发方式为0（边沿触发）时，单沿/双沿才有效。 */
		if ( single == 0 )
		  LPC_GPIO3->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO3->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO3->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO3->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO3->IEV |= (0x1<<bitPosi);	  
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** 函数名称:		GPIOIntEnable
**
** 功能描述:		使能端口引脚的中断屏蔽。
**
** 参数:			端口号、位位置
** 返回值:		无
** 
*****************************************************************************/
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IE |= (0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IE |= (0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IE |= (0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IE |= (0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** 函数名称:		GPIOIntDisable
**
** 功能描述:		禁用端口引脚的中断屏蔽。
**
** 参数:			端口号、位位置
** 返回值:		无
** 
*****************************************************************************/
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IE &= ~(0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IE &= ~(0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IE &= ~(0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IE &= ~(0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** 函数名称:		GPIOIntStatus
**
** 功能描述:		获取端口引脚的中断状态。
**
** 参数:			端口号、位位置
** 返回值:		无
** 
*****************************************************************************/
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi )
{
  uint32_t regVal = 0;

  switch ( portNum )
  {
	case PORT0:
	  if ( LPC_GPIO0->MIS & (0x1<<bitPosi) )
		regVal = 1;
	break;
 	case PORT1:
	  if ( LPC_GPIO1->MIS & (0x1<<bitPosi) )
		regVal = 1;	
	break;
	case PORT2:
	  if ( LPC_GPIO2->MIS & (0x1<<bitPosi) )
		regVal = 1;		    
	break;
	case PORT3:
	  if ( LPC_GPIO3->MIS & (0x1<<bitPosi) )
		regVal = 1;		    
	break;
	default:
	  break;
  }
  return ( regVal );
}

/*****************************************************************************
** 函数名称:		GPIOIntClear
**
** 功能描述:		清除端口引脚的中断。
**
** 参数:			端口号、位位置
** 返回值:		无
** 
*****************************************************************************/
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IC |= (0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IC |= (0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IC |= (0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IC |= (0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** 函数名称:		GPIOSetValue
**
** 功能描述:		在GPIO端口X（X为端口号）的特定位位置设置/清除位值
**
** 参数:			端口号、位位置、位值
** 返回值:		无
**
*****************************************************************************/
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
  LPC_GPIO[portNum]->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
}

/*****************************************************************************
** 函数名称:		GPIOSetDir
**
** 功能描述:		设置GPIO端口的方向
**
** 参数:			端口号、位位置、方向（1为输出，0为输入）
** 返回值:		无
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  if(dir)
	LPC_GPIO[portNum]->DIR |= 1<<bitPosi;
  else
	LPC_GPIO[portNum]->DIR &= ~(1<<bitPosi);
}

//====================================================================
//  函数：GPIOGetValue
//  描述：读取指定 GPIO 引脚的当前电平
//  参数：portNum - GPIO 端口号 (0~3)
//        bitPosi - 引脚位号 (0~31)
//  返回：0 = 低电平
//        1 = 高电平
//====================================================================
uint8_t GPIOGetValue(uint32_t portNum, uint32_t bitPosi)
{
    uint32_t value = 0;

    switch (portNum)
    {
        case PORT0:
            value = (LPC_GPIO0->DATA & (1UL << bitPosi)) ? 1 : 0;
            break;

        case PORT1:
            value = (LPC_GPIO1->DATA & (1UL << bitPosi)) ? 1 : 0;
            break;

        case PORT2:
            value = (LPC_GPIO2->DATA & (1UL << bitPosi)) ? 1 : 0;
            break;

        case PORT3:
            value = (LPC_GPIO3->DATA & (1UL << bitPosi)) ? 1 : 0;
            break;

        default:
            value = 0;
            break;
    }

    return (uint8_t)value;
}


#endif

/******************************************************************************
**                            文件结束
******************************************************************************/