#include "led.h"

void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);	//开启时钟
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//高电平，使LED灯灭
	LED_B_5 = 1;	//GPIO_SetBits(GPIOB, GPIO_Pin_5);
	LED_E_5 = 1;	
	LED_E_6 = 1;	//GPIO_SetBits(GPIOE, GPIO_Pin_5 | GPIO_Pin_6);
}
