#include "Key.h"
#include "driver_config.h"
#include "gpio.h"


volatile uint8_t key_flag = 0;

void KEY_IRQ_Init(void)
{
    GPIOInit();

    GPIOSetDir(PORT1, 0, 0);

    LPC_IOCON->R_PIO1_0 &= ~0x03;
    LPC_IOCON->R_PIO1_0 |= 0x01;

    GPIOSetInterrupt(PORT1, 0, 0, 0, 0);

    GPIOIntEnable(PORT1, 0);

    NVIC_EnableIRQ(EINT1_IRQn);
}

void PIOINT1_IRQHandler(void)
{
    if (GPIOIntStatus(PORT1, 0))
    {
        key_flag = 1;

        GPIOIntClear(PORT1, 0);
    }

}