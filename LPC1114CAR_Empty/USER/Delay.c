#include "Delay.h"
#include "timer16.h"

// 全局毫秒计数器
static volatile uint32_t sys_tick_count = 0;

/* 系统节拍定时器中断处理函数（每1ms进入一次） */
void SysTick_Handler(void) {
    if (sys_tick_count > 0) {
        sys_tick_count--;  // 每1ms递减计数器
    }
}

/**
 * 毫秒级延时函数
 * @param ms：需要延时的毫秒数（范围：1~0xFFFFFFFF）
 */
void delay_ms(uint32_t ms) {
    if (ms == 0) {
        return; 
    }
    
    sys_tick_count = ms;  
    while (sys_tick_count > 0); 
}