#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define KEY1  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define KEY2  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)

void Key_Init(void);
u8 Key_Scan(void);


#endif
