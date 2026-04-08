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

#ifndef __SPI1_H__
#define __SPI1_H__

#include "LPC11xx.h"
#include "driver_config.h"
#include "ssp.h"
#include "gpio.h"
#include <stddef.h>  // 添加此行，用于定义NULL
//SPI引脚定义
//#define SPI_CLK_GPIO_PORT                GPIOA
//#define SPI_CLK_GPIO_CLK                 RCC_APB2Periph_GPIOA
//#define SPI_CLK_GPIO_PIN                 GPIO_Pin_5

//#define SPI_MISO_GPIO_PORT               GPIOA
//#define SPI_MISO_GPIO_CLK                RCC_APB2Periph_GPIOA
//#define SPI_MISO_GPIO_PIN                GPIO_Pin_6

//#define SPI_MOSI_GPIO_PORT               GPIOA
//#define SPI_MOSI_GPIO_CLK                RCC_APB2Periph_GPIOA
//#define SPI_MOSI_GPIO_PIN                GPIO_Pin_7

//#define SPI_NSS_GPIO_PORT                GPIOA
//#define SPI_NSS_GPIO_CLK                 RCC_APB2Periph_GPIOA
//#define SPI_NSS_GPIO_PIN                 GPIO_Pin_4
// 引脚定义（根据需求：SSP1对应引脚）
#define SPI_SSP_NUM         1               // 使用SSP1
#define SPI_CS_PORT         PORT2           // PIO2_0对应PORT2
#define SPI_CS_PIN          0               // PIO2_0为CS引脚

#define spi_set_nss_high( )            	 GPIOSetValue(SPI_CS_PORT, SPI_CS_PIN, 1)  //片选置高
#define spi_set_nss_low( )               GPIOSetValue(SPI_CS_PORT, SPI_CS_PIN, 0) //片选置低



///******** 硬件SPI修改此次 ********/
//#define RCU_SPI_HARDWARE 	RCC_APB2Periph_SPI1
//#define PORT_SPI         	SPI1


void drv_spi_init( void );
uint8_t drv_spi_read_write_byte( uint8_t TxByte );
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length );

#endif


