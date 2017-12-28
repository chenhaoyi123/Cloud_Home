#include "dht11.h"

void DHT11_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed 	=  GPIO_Speed_50MHz;\
	
	GPIO_Init(GPIOA, & GPIO_InitStructure);
}


//void DHT11_int(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	
//	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode	= 
//}
