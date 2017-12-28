#ifndef __USART3_H_
#define __USART3_H_

#include "stm32f10x.h"

#define USART3_MAX_RECV_LEN		2048		//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		2048		//最大发送缓存字节数

extern u8  USART3_RX_FLAG;		//1表示串口接收完数据 0无数据
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 			//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 
extern u16 USART3_RX_STA;

void Usart3_Init(uint32_t BaudRate);
void Usart3_Send_Byte(unsigned char SendData);
void Usart3_Send_String(u8 *Data, u16 len);

#endif
