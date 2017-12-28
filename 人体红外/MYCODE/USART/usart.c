#include "usart.h"
#include "stdio.h"

void Usart1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	//使用的是串口 1，串口 1 对应着芯片引脚 PA9,PA10 需要使能PA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            
  GPIO_Init(GPIOA, &GPIO_InitStructure);     
	
	
	
	//波特率设置;
	USART_InitStructure.USART_BaudRate 		= 115200;   
	//字长为 8 位数据格式
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	//一个停止位
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	//无奇偶校验位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	//硬件控制流
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//收发模式，收发模式
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//初始化串口
	USART_Init(USART1, &USART_InitStructure); 
	
	//使能串口
	USART_Cmd(USART1, ENABLE); 

	//开启接收中断
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
	//IRQ 通道使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	//根据指定的参数初始化 VIC 寄存器
	NVIC_Init(&NVIC_InitStructure);  

}
