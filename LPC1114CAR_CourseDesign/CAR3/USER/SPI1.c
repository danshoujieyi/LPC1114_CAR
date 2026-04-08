/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-04-01     LCKFB-LP    first version
 */

#include "SPI1.h"
      


/** 硬件SPI */
#define SPI_WAIT_TIMEOUT       ((uint16_t)0xFFFF)

/**
  * @brief :SPI初始化(硬件)
  * @param :无
  * @note  :无
  * @retval:无
  */ 
void drv_spi_init( void )
{
    // 1. 配置SSP1引脚（PIO2_0=CS, PIO2_1=SCK, PIO2_2=MISO, PIO2_3=MOSI）
    // SSP_IOConfig会自动根据portNum=1配置上述引脚
    SSP_IOConfig(SPI_SSP_NUM);
    
    // 2. 初始化SSP1外设
    // 配置说明：主机模式、8位数据、CPOL=0、CPHA=0（与原代码一致）
    SSP_Init(SPI_SSP_NUM);
	
	spi_set_nss_high();

}

/**
  * @brief :SPI收发一个字节
  * @param :
  *                        @TxByte: 发送的数据字节
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:接收到的字节
  */
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
    uint8_t rx_data = 0;
    uint16_t wait_time = 0;
    LPC_SSP_TypeDef *pSSP = (SPI_SSP_NUM == 0) ? LPC_SSP0 : LPC_SSP1;

    // 等待发送FIFO非满且不忙
    while ((pSSP->SR & (SSPSR_TNF | SSPSR_BSY)) != SSPSR_TNF)
    {
        if (++wait_time >= SPI_WAIT_TIMEOUT)
            return 0; // 超时退出
    }
    wait_time = 0;

    // 发送数据
    pSSP->DR = TxByte;

    // 等待接收FIFO非空且不忙
    while ((pSSP->SR & (SSPSR_RNE | SSPSR_BSY)) != SSPSR_RNE)
    {
        if (++wait_time >= SPI_WAIT_TIMEOUT)
            return 0; // 超时退出
    }

    // 读取接收数据
    rx_data = pSSP->DR;
    return rx_data;
}

/**
  * @brief :SPI收发字符串
  * @param :
  *                        @ReadBuffer: 接收数据缓冲区地址
  *                        @WriteBuffer:发送字节缓冲区地址
  *                        @Length:字节长度
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:无
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
    if (ReadBuffer == NULL || WriteBuffer == NULL || Length == 0)
        return;

    spi_set_nss_low();  // 拉低片选

    while (Length--)
    {
        *ReadBuffer++ = drv_spi_read_write_byte(*WriteBuffer++);
    }

    spi_set_nss_high(); // 拉高片选
}




