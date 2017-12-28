#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include "stm32f10x.h"

void Systick_Init(void); 	
void delay_us(u32 nus);				
void delay_ms(u32 nms);		
void delay_s(u32 ns);		


#endif
