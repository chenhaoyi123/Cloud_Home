/****************************************** 
 * 描述    ：配置蜂鸣器          
 * 实验平台：STM32开发板 基于STM32F103ZET6 
 * 硬件连接：	 ------------------- 
 *          	| PA9  - USART1(Tx) | 
 *          	| PA10 - USART1(Rx) | 
 *           	 -------------------  
*******************************************/

#include "beep.h"

void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
	
	
}
