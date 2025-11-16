/****************************************************************************
 *   $Id:: timer16.h 4785 2010-09-03 22:39:27Z nxp21346                     $
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
#ifndef __TIMER16_H 
#define __TIMER16_H 
#if CONFIG_ENABLE_DRIVER_TIMER16==1

/* TIMER_CLOCKFREQ是进入定时器预分频器的时钟频率 */
#define TIMER_CLOCKFREQ SystemCoreClock

/* MHZ_PRESCALE是设置预分频器的值，用于将定时器时钟设置为1MHz。
   时钟必须是1MHz的倍数，否则此设置无效。 */
#define MHZ_PRESCALE    (TIMER_CLOCKFREQ/1000000)

/* TIME_INTERVALmS是加载到定时器匹配寄存器的值，用于获得1毫秒的延迟 */
#define TIME_INTERVALmS	1000

/* 测试模式可以是MAT_OUT（匹配输出）或CAP_IN（捕获输入）。默认是MAT_OUT。 */
#define TIMER_MATCH		0

void delayMs(uint8_t timer_num, uint32_t delayInMs);

#define EMC0	4
#define EMC1	6
#define EMC2	8
#define EMC3	10

#define MATCH0	(1<<0)
#define MATCH1	(1<<1)
#define MATCH2	(1<<2)
#define MATCH3	(1<<3)

void TIMER16_0_IRQHandler(void);
void TIMER16_1_IRQHandler(void);

void enable_timer16(uint8_t timer_num);
void disable_timer16(uint8_t timer_num);
void reset_timer16(uint8_t timer_num);
void init_timer16(uint8_t timer_num, uint16_t timerInterval);
void init_timer16PWM(uint8_t timer_num, uint32_t period, uint8_t match_enable, uint8_t cap_enabled);
void setMatch_timer16PWM (uint8_t timer_num, uint8_t match_nr, uint32_t value);

#endif
#endif /* end __TIMER16_H */
/*****************************************************************************
**                            文件结束
******************************************************************************/