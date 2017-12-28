#include "delay.h"

static u8  my_us=0;	//us延时倍乘数			   
static u16 my_ms=0;	//ms延时倍乘数

/*************************************
 *	SYSTICK的时钟固定为HCLK时钟的1/8
 *	SYSCLK:系统时钟
**************************************/
void Delay_init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	my_us = SystemCoreClock/8000000;						//为系统时钟的1/8  

	my_ms=(u16)my_us*1000;					//非ucos下,代表每个ms需要的systick时钟数    
}								    

/***********************
 *	延时nus
 *	nus为要延时的微妙数.	
************************/
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD  = nus*my_us; 				//时间加载	  		 
	SysTick->VAL   = 0x00;        				//清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp = SysTick->CTRL;					//CTRL：SYSTick的控制和状态寄存器
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL = 0X00;      					//清空计数器	 
}

/**********************************************
 *	延时nms
 *	注意nms的范围
 *	SysTick->LOAD为24位寄存器,所以,最大延时为：nms<=0xffffff*8*1000/SYSCLK
 *	SYSCLK单位为Hz,nms单位为ms
 *	对72M条件下,nms<=1864 
***********************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD  = (u32)nms*my_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL   = 0x00;						//清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL = 0X00;       					//清空计数器	  	    
} 

/***********************
 *	延时ns
 *	ns为要延时的秒数.	
************************/
void delay_s(u16 ns)
{
	int i;
	for(i = 0; i < ns; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}
		
}
