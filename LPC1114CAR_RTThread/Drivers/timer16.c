/****************************************************************************
 *   $Id:: timer16.c 4785 2010-09-03 22:39:27Z nxp21346                     $
 *   项目：NXP LPC11xx 16位定时器示例
 *
 *   描述：
 *     本文件包含16位定时器代码示例，包括定时器初始化、定时器中断处理程序以及
 *     定时器设置的相关API。
 *
 ****************************************************************************
 * 本文所述软件仅用于说明目的，为客户提供有关产品的编程信息。
 * 本软件按"原样"提供，不附带任何担保。NXP半导体不对软件的使用承担任何责任或义务，
 * 不授予任何专利、版权或掩模作品权下的许可或所有权。
 * NXP半导体保留在不通知的情况下修改软件的权利。
 * NXP半导体也不保证此类应用在未经进一步测试或修改的情况下适用于特定用途。
****************************************************************************/
#include "driver_config.h"
#include "timer16.h"

#if CONFIG_ENABLE_DRIVER_TIMER16==1

volatile uint32_t timer16_0_counter = 0;
volatile uint32_t timer16_1_counter = 0;
volatile uint32_t timer16_0_capture = 0;
volatile uint32_t timer16_1_capture = 0;
volatile uint32_t timer16_0_period = 0;
volatile uint32_t timer16_1_period = 0;

/*****************************************************************************
** 函数名称:		delayMs
**
** 功能描述:		启动定时器延迟（毫秒级），直到延迟结束
**
** 参数:			timer number：定时器编号，delayInMs：延迟的毫秒值			 
** 						
** 返回值:		无
** 
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if (timer_num == 0)
  {
    /*
    * 配置定时器#0用于延迟
    */
    LPC_TMR16B0->TCR = 0x02;		/* 重置定时器 */
    LPC_TMR16B0->PR  = MHZ_PRESCALE; /* 设置预分频器以获得1M计数/秒 */
    LPC_TMR16B0->MR0 = delayInMs * TIME_INTERVALmS;
    LPC_TMR16B0->IR  = 0xff;		/* 重置所有中断 */
    LPC_TMR16B0->MCR = 0x04;		/* 匹配时停止定时器 */
    LPC_TMR16B0->TCR = 0x01;		/* 启动定时器 */
    /* 等待延迟时间结束 */
    while (LPC_TMR16B0->TCR & 0x01);
  }
  else if (timer_num == 1)
  {
    /*
    * 配置定时器#1用于延迟
    */
    LPC_TMR16B1->TCR = 0x02;		/* 重置定时器 */
    LPC_TMR16B1->PR  = MHZ_PRESCALE; /* 设置预分频器以获得1M计数/秒 */
    LPC_TMR16B1->MR0 = delayInMs * TIME_INTERVALmS;
    LPC_TMR16B1->IR  = 0xff;		/* 重置所有中断 */
    LPC_TMR16B1->MCR = 0x04;		/* 匹配时停止定时器 */
    LPC_TMR16B1->TCR = 0x01;		/* 启动定时器 */
    /* 等待延迟时间结束 */
    while (LPC_TMR16B1->TCR & 0x01);
  }
  return;
}

/******************************************************************************
** 函数名称:		TIMER16_0_IRQHandler
**
** 功能描述:		定时器/计数器0中断处理程序
**						在60MHz CPU时钟下每10ms执行一次
**
** 参数:			无
** 返回值:		无
** 
******************************************************************************/
void TIMER16_0_IRQHandler(void)
{  
  if ( LPC_TMR16B0->IR & 0x1 )
  {
  LPC_TMR16B0->IR = 1;			/* 清除中断标志 */
  timer16_0_counter++;
  }
  if ( LPC_TMR16B0->IR & (0x1<<4) )
  {
	LPC_TMR16B0->IR = 0x1<<4;		/* 清除中断标志 */
	timer16_0_capture++;
  }
  return;
}

/******************************************************************************
** 函数名称:		TIMER16_1_IRQHandler
**
** 功能描述:		定时器/计数器1中断处理程序
**						在60MHz CPU时钟下每10ms执行一次
**
** 参数:			无
** 返回值:		无
** 
******************************************************************************/
void TIMER16_1_IRQHandler(void)
{  
  if ( LPC_TMR16B1->IR & 0x1 )
  {  
  LPC_TMR16B1->IR = 1;			/* 清除中断标志 */
  timer16_1_counter++;
  }
  if ( LPC_TMR16B1->IR & (0x1<<4) )
  {
	LPC_TMR16B1->IR = 0x1<<4;		/* 清除中断标志 */
	timer16_1_capture++;
  }
  return;
}

/******************************************************************************
** 函数名称:		enable_timer16
**
** 功能描述:		使能定时器
**
** 参数:			timer number：定时器编号（0或1）
** 返回值:		无
** 
******************************************************************************/
void enable_timer16(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_TMR16B0->TCR = 1;
  }
  else
  {
    LPC_TMR16B1->TCR = 1;
  }
  return;
}

/******************************************************************************
** 函数名称:		disable_timer16
**
** 功能描述:		禁用定时器
**
** 参数:			timer number：定时器编号（0或1）
** 返回值:		无
** 
******************************************************************************/
void disable_timer16(uint8_t timer_num)
{
  if ( timer_num == 0 )
  {
    LPC_TMR16B0->TCR = 0;
  }
  else
  {
    LPC_TMR16B1->TCR = 0;
  }
  return;
}

/******************************************************************************
** 函数名称:		reset_timer16
**
** 功能描述:		重置定时器
**
** 参数:			timer number：定时器编号（0或1）
** 返回值:		无
** 
******************************************************************************/
void reset_timer16(uint8_t timer_num)
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
    regVal = LPC_TMR16B0->TCR;
    regVal |= 0x02;
    LPC_TMR16B0->TCR = regVal;
  }
  else
  {
    regVal = LPC_TMR16B1->TCR;
    regVal |= 0x02;
    LPC_TMR16B1->TCR = regVal;
  }
  return;
}

/******************************************************************************
** 函数名称:		init_timer16
**
** 功能描述:		初始化定时器，设置定时器间隔，重置定时器，
**						安装定时器中断处理程序
**
** 参数:			timer number：定时器编号，timerInterval：定时器间隔
** 返回值:		无
** 
******************************************************************************/
void init_timer16(uint8_t timer_num, uint16_t TimerInterval)
{
  if ( timer_num == 0 )
  {
    /* 如果使用以下模块，需要仔细规划部分I/O引脚，
    因为JTAG和定时器CAP/MAT引脚是复用的。 */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
    LPC_IOCON->PIO0_2           &= ~0x07;	/*  定时器0_16 I/O配置 */
    LPC_IOCON->PIO0_2           |= 0x02;		/* 定时器0_16 CAP0 */
    LPC_IOCON->PIO0_8           &= ~0x07;	
    LPC_IOCON->PIO0_8           |= 0x02;		/* 定时器0_16 MAT0 */
    LPC_IOCON->PIO0_9           &= ~0x07;
    LPC_IOCON->PIO0_9           |= 0x02;		/* 定时器0_16 MAT1 */
#ifdef __JTAG_DISABLED
    LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;
    LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x03;		/* 定时器0_16 MAT2 */
#endif	

    timer16_0_counter = 0;
	timer16_0_capture = 0;

    LPC_TMR16B0->PR  = MHZ_PRESCALE; /* 设置预分频器以获得1M计数/秒 */
    LPC_TMR16B0->MR0 = TIME_INTERVALmS * 10; /* 设置10ms间隔 */
#if TIMER_MATCH
	LPC_TMR16B0->EMR &= ~(0xFF<<4);
	LPC_TMR16B0->EMR |= ((0x3<<4)|(0x3<<6));
#else
	/* 上升沿捕获0，使能中断。 */
	LPC_TMR16B0->CCR = (0x1<<0)|(0x1<<2);
#endif
    LPC_TMR16B0->MCR = 3;				/* 在MR0和MR1上产生中断并重置 */

    /* 使能TIMER0中断 */
    NVIC_EnableIRQ(TIMER_16_0_IRQn);
  }
  else if ( timer_num == 1 )
  {
    /* 如果使用以下模块，需要仔细规划部分I/O引脚，
    因为JTAG和定时器CAP/MAT引脚是复用的。 */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
    LPC_IOCON->PIO1_8           &= ~0x07;	/*  定时器1_16 I/O配置 */
    LPC_IOCON->PIO1_8           |= 0x01;		/* 定时器1_16 CAP0 */
    LPC_IOCON->PIO1_9           &= ~0x07;	
    LPC_IOCON->PIO1_9           |= 0x01;		/* 定时器1_16 MAT0 */
    LPC_IOCON->PIO1_10          &= ~0x07;
    LPC_IOCON->PIO1_10          |= 0x02;		/* 定时器1_16 MAT1 */	

    timer16_1_counter = 0;
	timer16_1_capture = 0;
    LPC_TMR16B0->PR  = MHZ_PRESCALE; /* 设置预分频器以获得1M计数/秒 */
    LPC_TMR16B0->MR0 = TIME_INTERVALmS * 10; /* 设置10ms间隔 */
#if TIMER_MATCH
	LPC_TMR16B1->EMR &= ~(0xFF<<4);
	LPC_TMR16B1->EMR |= ((0x3<<4)|(0x3<<6));
#else
	/* 上升沿捕获0，使能中断。 */
	LPC_TMR16B1->CCR = (0x1<<0)|(0x1<<2);
#endif
    LPC_TMR16B1->MCR = 3;				/* 在MR0和MR1上产生中断并重置 */

    /* 使能TIMER1中断 */
    NVIC_EnableIRQ(TIMER_16_1_IRQn);
  }
  return;
}
/******************************************************************************
** 函数名称:		init_timer16PWM
**
** 功能描述:		将定时器初始化为PWM模式
**
** 参数:			timer number：定时器编号，period：周期，match_enable：
**										match_enable[0] = MAT0的PWM使能
**										match_enable[1] = MAT1的PWM使能
**										match_enable[2] = MAT2的PWM使能
**			
** 返回值:		无
** 
******************************************************************************/
void init_timer16PWM(uint8_t timer_num, uint32_t period, uint8_t match_enable, uint8_t cap_enabled)
{
	
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	disable_timer16(timer_num);

	if (timer_num == 1)
	{

		/* 如果使用以下模块，需要仔细规划部分I/O引脚，
		因为JTAG和定时器CAP/MAT引脚是复用的。 */
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
		
		/* 配置外部匹配寄存器 */
		LPC_TMR16B1->EMR = (1<<EMC3)|(1<<EMC2)|(1<<EMC1)|(2<<EMC0)|(1<<3)|(match_enable);
		
		/* 配置输出 */
		/* 如果match0使能，设置输出 */
		if (match_enable & 0x01)
		{
			LPC_IOCON->PIO1_9           &= ~0x07;	
			LPC_IOCON->PIO1_9           |= 0x01;		/* 定时器1_16 MAT0 */
		}
		/* 如果match1使能，设置输出 */
		if (match_enable & 0x02)
		{
			LPC_IOCON->PIO1_10          &= ~0x07;
			LPC_IOCON->PIO1_10          |= 0x02;		/* 定时器1_16 MAT1 */
		}
		
		/* 使能选中的PWM并使能Match3 */
		LPC_TMR16B1->PWMC = (1<<3)|(match_enable);
		
		/* 配置匹配寄存器 */
		/* 将周期值设置到全局变量 */
		timer16_1_period 	= period;
		LPC_TMR16B1->MR3 	= timer16_1_period;
		LPC_TMR16B1->MR0	= timer16_1_period/2;
		LPC_TMR16B1->MR1	= timer16_1_period/2;
		LPC_TMR16B1->MR2	= timer16_1_period/2;
		
		/* 设置匹配控制寄存器 */
		LPC_TMR16B1->MCR = 1<<10;// | 1<<9;				/* 在MR3上重置 */
		
		if (cap_enabled)
		{
			LPC_IOCON->PIO1_8 &= ~0x07;						/*  定时器1_16 I/O配置 */
			LPC_IOCON->PIO1_8 |= 0x01 | (2<<3);				/* 定时器1_16 CAP0 */
			LPC_GPIO1->DIR &= ~(1<<8); 
			LPC_TMR16B1->IR = 0xF;							/* 清除中断标志 */
			
			/* 设置捕获控制寄存器 */
			LPC_TMR16B1->CCR = 7;
			
		}
		/* 使能TIMER1中断 */
		NVIC_EnableIRQ(TIMER_16_1_IRQn);
	}
	else
	{
		/* 如果使用以下模块，需要仔细规划部分I/O引脚，
		因为JTAG和定时器CAP/MAT引脚是复用的。 */
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);
		
		/* 配置外部匹配寄存器 */
		LPC_TMR16B0->EMR = (1<<EMC3)|(1<<EMC2)|(1<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);
		
		/* 配置输出 */
		/* 如果match0使能，设置输出 */
		if (match_enable & 0x01)
		{
			LPC_IOCON->PIO0_8           &= ~0x07;	
			LPC_IOCON->PIO0_8           |= 0x02;		/* 定时器0_16 MAT0 			*/
		}
		/* 如果match1使能，设置输出 */
		if (match_enable & 0x02)
		{
			LPC_IOCON->PIO0_9           &= ~0x07;
			LPC_IOCON->PIO0_9           |= 0x02;		/* 定时器0_16 MAT1 			*/
		}
		/* 如果match2使能，设置输出 */
		if (match_enable & 0x04)
		{
			LPC_IOCON->SWCLK_PIO0_10 &= ~0x07;
			LPC_IOCON->SWCLK_PIO0_10 |= 0x03;		/* 定时器0_16 MAT2 */
		}
		
		//	  PIO0_2           &= ~0x07;	/* 定时器0_16 I/O配置 */
		//	  PIO0_2           |= 0x02;		/* 定时器0_16 CAP0 			*/
		
		/* 使能选中的PWM并使能Match3 */
		LPC_TMR16B0->PWMC = (1<<3)|(match_enable);
		
		/* 配置匹配寄存器 */
		/* 将周期值设置到全局变量 */
		timer16_0_period = period;
		LPC_TMR16B0->MR3 = timer16_0_period;
		LPC_TMR16B0->MR0	= timer16_0_period/2;
		LPC_TMR16B0->MR1	= timer16_0_period/2;
		LPC_TMR16B0->MR2	= timer16_0_period/2;
		
		/* 设置匹配控制寄存器 */
		LPC_TMR16B0->MCR = 1<<10;				/* 在MR3上重置 */
		
		/* 使能TIMER1中断 */
		NVIC_EnableIRQ(TIMER_16_0_IRQn);
	}

}
/******************************************************************************
** 函数名称:		setMatch_timer16PWM
**
** 功能描述:		设置pwm16的匹配值
**
** 参数:			timer number：定时器编号，match nr：匹配编号，value：匹配值
**
** 返回值:		无
** 
******************************************************************************/
void setMatch_timer16PWM (uint8_t timer_num, uint8_t match_nr, uint32_t value)
{
	if (timer_num)
	{
		switch (match_nr)
		{
			case 0:
				LPC_TMR16B1->MR0 = value;
				break;
			case 1: 
				LPC_TMR16B1->MR1 = value;
				break;
			case 2:
				LPC_TMR16B1->MR2 = value;
				break;
			case 3: 
				LPC_TMR16B1->MR3 = value;
				break;
			default:
				break;
		}	

	}
	else 
	{
		switch (match_nr)
		{
			case 0:
				LPC_TMR16B0->MR0 = value;
				break;
			case 1: 
				LPC_TMR16B0->MR1 = value;
				break;
			case 2:
				LPC_TMR16B0->MR2 = value;
				break;
			case 3: 
				LPC_TMR16B0->MR3 = value;
				break;
			default:
				break;
		}	
	}

}

#endif
/******************************************************************************
**                            文件结束
******************************************************************************/