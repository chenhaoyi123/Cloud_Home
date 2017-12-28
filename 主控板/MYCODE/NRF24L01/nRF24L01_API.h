#ifndef __nRF24L01_API_H
#define __nRF24L01_API_H

#include "nRF24L01.h"
#include "stm32f10x.h"


//nrf24l01操作
#define NRF24L01_CE   PCout(4) //24L01片选信号
#define NRF24L01_CSN  PAout(4) //SPI片选信号      
#define NRF24L01_IRQ  PCin(5)  //IRQ主机数据输入

//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5       //5字节的地址宽度
#define RX_ADR_WIDTH    5       //5字节的地址宽度
#define TX_PLOAD_WIDTH  32      //32字节的用户数据宽度
#define RX_PLOAD_WIDTH  32      //32字节的用户数据宽度

void NRF24L01_Init(void);
u8 SPI1_ReadWriteByte(u8 TxData);
u8 NRF24L01_Write_Reg(u8 reg,u8 value);
u8 NRF24L01_Read_Reg(u8 reg);
void NRF24L01_RX_Mode(u8* Address);
void NRF24L01_TX_Mode(u8* Address);
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len);
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len);
u8 NRF24L01_TxPacket(u8 *txbuf);
u8 NRF24L01_RxPacket(u8 *rxbuf);
u8 NRF24L01_Check(void);

#endif
