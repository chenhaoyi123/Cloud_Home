#ifndef __USART3_H_
#define __USART3_H_

#include "stm32f10x.h"

#define USART3_MAX_RECV_LEN		2048		//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		2048		//����ͻ����ֽ���

extern u8  USART3_RX_FLAG;		//1��ʾ���ڽ��������� 0������
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 			//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 
extern u16 USART3_RX_STA;

void Usart3_Init(uint32_t BaudRate);
void Usart3_Send_Byte(unsigned char SendData);
void Usart3_Send_String(u8 *Data, u16 len);

#endif
