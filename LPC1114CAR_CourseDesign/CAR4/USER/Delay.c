#include "Delay.h"
#include "timer16.h"
///******************************************************************************/
///**
// * 函数功能：初始化16位定时器0
// * 描述：配置并初始化16位定时器0，设置定时器的时钟源、计数器模式和匹配寄存器，确保定时器可以正常工作。
// */
//void T16B0_init(void)
//{
//    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);  // 使能16位定时器0的时钟（TIM16B0）
//    LPC_TMR16B0->IR = 0x01;                 // 清除定时器0的中断标志，设置中断源为MR0（匹配0寄存器）
//    LPC_TMR16B0->MCR = 0x04;                // 配置匹配控制寄存器（MCR）：匹配0后重置TC，不生成中断（TCR[0]=0）
//}

///**
// * 函数功能：延时函数（毫秒级）
// * 描述：使用16位定时器0生成精确的毫秒级延时。
// * 参数：
// *      ms - 延迟的毫秒数（最大65535ms，即65.535秒）。
// */
//void T16B0_delay_ms(uint16_t ms)
//{
//    LPC_TMR16B0->TCR = 0x02;                    // 重置定时器（TCR[1] = 1），停止定时器
//    LPC_TMR16B0->PR = SystemCoreClock / 1000 - 1; // 设置定时器预分频器，确保定时器计数器按1ms递增
//    LPC_TMR16B0->MR0 = ms;                      // 设置MR0为目标延时值（单位：毫秒）
//    LPC_TMR16B0->TCR = 0x01;                    // 启动定时器（TCR[0] = 1）
//    LPC_TMR16B0->TCR = 0x01;                    // 启动定时器（TCR[0] = 1），重新启动
//    while (LPC_TMR16B0->TCR & 0x01);            // 等待定时器计数完成，直到TCR[0]为0，表示延时结束
//}

///**
// * 函数功能：延时函数（微秒级）
// * 描述：使用16位定时器0生成精确的微秒级延时。
// * 参数：
// *      us - 延迟的微秒数（最大65535us，即65.535ms）。
// */
//void T16B0_delay_us(uint16_t us)
//{
//    LPC_TMR16B0->TCR = 0x02;                    // 重置定时器（TCR[1] = 1），停止定时器
//    LPC_TMR16B0->PR = SystemCoreClock / 1000000 - 1; // 设置定时器预分频器，确保定时器计数器按1us递增
//    LPC_TMR16B0->MR0 = us;                      // 设置MR0为目标延时值（单位：微秒）
//    LPC_TMR16B0->TCR = 0x01;                    // 启动定时器（TCR[0] = 1）
//    while (LPC_TMR16B0->TCR & 0x01);            // 等待定时器计数完成，直到TCR[0]为0，表示延时结束
//}

//// 全局毫秒计数器（volatile确保不被编译器优化，因为在中断中修改）
//static volatile uint32_t sys_tick_count = 0;

///* 系统节拍定时器中断处理函数（每1ms进入一次） */
//void SysTick_Handler(void) {
//    if (sys_tick_count > 0) {
//        sys_tick_count--;  // 每1ms递减计数器
//    }
//}

///**
// * 毫秒级延时函数
// * @param ms：需要延时的毫秒数（范围：1~0xFFFFFFFF）
// */
//void delay_ms(uint32_t ms) {
//    if (ms == 0) {
//        return;  // 延时0ms直接返回
//    }
//    
//    sys_tick_count = ms;  // 设置计数器初始值为目标延时时长
//    while (sys_tick_count > 0);  // 等待计数器被中断减到0
//}