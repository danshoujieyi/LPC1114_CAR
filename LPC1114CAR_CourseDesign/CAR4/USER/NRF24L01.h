

#ifndef __NRF24L01_H__
#define __NRF24L01_H__


#include "driver_config.h"
#include "ssp.h"
#include "gpio.h"
#include "NRF24L01REG.h"
#include "LPC11xx.h"    


//extern uint8_t NRF24L01_TxAddress[];
//extern uint8_t NRF24L01_TxPacket[];
//extern uint8_t NRF24L01_RxAddress[];
//extern uint8_t NRF24L01_RxPacket[];
 
uint8_t NRF24L01_ReadReg(uint8_t RegAddress);
void NRF24L01_ReadRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteReg(uint8_t RegAddress, uint8_t Data);
void NRF24L01_WriteRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_ReadRxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteTxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_FlushTx(void);
void NRF24L01_FlushRx(void);
uint8_t NRF24L01_ReadStatus(void);
void NRF24L01_PowerDown(void);
void NRF24L01_StandbyI(void);
void NRF24L01_RxMode(void);
void NRF24L01_TxMode(void);
void NRF24L01_Init(void);
uint8_t NRF24L01_Send(void);


uint8_t NRF24L01_Receive(void);

#endif

