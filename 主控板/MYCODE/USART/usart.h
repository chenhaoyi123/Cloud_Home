#ifndef __USART_H_
#define __USART_H_

#include "stm32f10x.h"

void Usart1_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);

#endif
