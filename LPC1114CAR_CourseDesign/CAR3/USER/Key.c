#include "Key.h"
#include "driver_config.h"
#include "gpio.h"


volatile uint8_t key_flag = 0;

// 初始化PIO1_0为中断按键输入（上拉输入，下降沿触发）
void KEY_IRQ_Init(void)
{
    // 1. 初始化GPIO模块（使能时钟及基础配置）
    GPIOInit();

    // 2. 设置PIO1_0为输入模式
    GPIOSetDir(PORT1, 0, 0);  // PORT1，引脚0，输入模式（0表示输入）

    // 3. 配置PIO1_0为上拉输入（按键未按下时为高电平）
    // LPC1114的PIO1_0通过IOCON寄存器配置上下拉（bit1-0：01=上拉）
    LPC_IOCON->R_PIO1_0 &= ~0x03;  // 清除原有配置
    LPC_IOCON->R_PIO1_0 |= 0x01;   // 使能上拉电阻

    // 4. 配置中断触发方式（下降沿触发，对应按键按下：高→低）
    // 参数：portNum=PORT1，bitPosi=0，sense=0（边沿触发），single=0（单边沿），event=0（下降沿）
    GPIOSetInterrupt(PORT1, 0, 0, 0, 0);

    // 5. 使能PIO1_0的中断
    GPIOIntEnable(PORT1, 0);

    // 6. 使能NVIC中PORT1对应的中断通道（EINT1_IRQn）
    // 注：GPIOInit()中可能已使能，此处显式确认
    NVIC_EnableIRQ(EINT1_IRQn);
}

// 中断服务函数：处理PORT1的中断（仅响应PIO1_0）
void PIOINT1_IRQHandler(void)
{
    // 检查是否是PIO1_0触发的中断
    if (GPIOIntStatus(PORT1, 0))
    {
        key_flag = 1;  // 主循环可检测该标志执行后续操作

        // 清除PIO1_0的中断标志（必须执行，否则重复触发）
        GPIOIntClear(PORT1, 0);
    }

    // 若需保留原PIO1_1的处理（如默认计数器），可添加以下代码（可选）
    // else if (GPIOIntStatus(PORT1, 1))
    // {
    //     p1_1_counter++;
    //     GPIOIntClear(PORT1, 1);
    // }
}