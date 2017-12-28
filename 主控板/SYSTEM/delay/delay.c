#include "delay.h"

static u8  my_us=0;	//us��ʱ������			   
static u16 my_ms=0;	//ms��ʱ������

/*************************************
 *	SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
 *	SYSCLK:ϵͳʱ��
**************************************/
void Delay_init()
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	my_us = SystemCoreClock/8000000;						//Ϊϵͳʱ�ӵ�1/8  

	my_ms=(u16)my_us*1000;					//��ucos��,����ÿ��ms��Ҫ��systickʱ����    
}								    

/***********************
 *	��ʱnus
 *	nusΪҪ��ʱ��΢����.	
************************/
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD  = nus*my_us; 				//ʱ�����	  		 
	SysTick->VAL   = 0x00;        				//��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp = SysTick->CTRL;					//CTRL��SYSTick�Ŀ��ƺ�״̬�Ĵ���
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL = 0X00;      					//��ռ�����	 
}

/**********************************************
 *	��ʱnms
 *	ע��nms�ķ�Χ
 *	SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ��nms<=0xffffff*8*1000/SYSCLK
 *	SYSCLK��λΪHz,nms��λΪms
 *	��72M������,nms<=1864 
***********************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD  = (u32)nms*my_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL   = 0x00;						//��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL = 0X00;       					//��ռ�����	  	    
} 

/***********************
 *	��ʱns
 *	nsΪҪ��ʱ������.	
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
