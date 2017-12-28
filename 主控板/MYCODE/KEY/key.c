#include "key.h"
#include "delay.h"

//S1->PE2, S2->PE3, S3->PE4
void Key_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//初始化时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;					//浮空输入
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//按键扫描函数
u8 Key_Scan(void)
{
	//判断按键KEY0是否按下
	if(KEY0 == 0)  //0是按下,看图示分析低电平为按下
	{
		delay_ms(10);	//消抖
		if(KEY0 == 0)
			return 0;  //成功按下，则返回0
	}
	
	//判断按键KEY1是否按下
	if(KEY1 == 0)  //0是按下,看图示分析
	{
		delay_ms(10);
		if(KEY1 == 0)
			return 1;
	}
	
	//判断按键KEY2是否按下
	if(KEY2 == 0)  //0是按下,看图示分析
	{
		delay_ms(10);
		if(KEY2 == 0)
			return 2;
	}
	
	return 3;
}
