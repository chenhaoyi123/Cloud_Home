#include "systick.h"

u8  my_us = 0;
u32 my_ms = 0;

void Systick_Init(void)
{
	//SysTick is 72MHZ/8 = 9MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	my_us = 72/8;  	//9
	my_ms = 9000;  //9000

}

void delay_us(u32 nus)
{
	u32 temp;
	//重装载值
	SysTick->LOAD =  nus*my_us;  //nus * 9
	SysTick->VAL  = 0x00;        //当前数值寄存器为0
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//定时器使能
	
	do
	{
		temp = SysTick->CTRL;
	
	}while((temp&0x01) && !(temp & (1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //定时器不使能
	
	SysTick->VAL  = 0x00;        //当前数值寄存器为0
}

void delay_ms(u32 nms)
{
	u32 temp;
	//重装载值
	SysTick->LOAD =  nms*my_ms;  //nus * 9
	SysTick->VAL  = 0x00;        //当前数值寄存器为0
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//定时器使能
	
	do
	{
		temp = SysTick->CTRL;
	
	}while((temp&0x01) && !(temp & (1<<16))); //判断寄存第十六位的值
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //定时器不使能
	
	SysTick->VAL  = 0x00;        //当前数值寄存器为0
}

void delay_s(u32 ns)
{
	for(; ns>0; ns--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
