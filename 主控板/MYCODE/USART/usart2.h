#ifndef __USART2_H_
#define __USART2_H_

#define USART2_MAX_RECV_LEN		2048		//最大接收缓存字节数

#include "stm32f10x.h"
#include "string.h"

extern u8  USART2_RX_FLAG;			//1表示串口接收完数据 0无数据
extern u8  rx_buffer[500];

void Usart2_Init(uint32_t band);
void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(uint32_t BaudRate);
void NVIC_Config(void);
void Usart2_Send_Byte(unsigned char SendData);
void Usart2_Send_String(char *Data, u16 len);

#endif
