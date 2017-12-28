#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	//向串口1发送数据
	USART_SendData(USART1, (u8)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	
	return ch;
}

//串口1中断服务程序
void USART1_IRQHandler(void)
{
	u8 Usart_Data;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//接收到数据
	{	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		/* DR读取接收到的数据 */
		Usart_Data = USART_ReceiveData(USART1);

		//收到数据就直接发送到串口1
		USART_SendData(USART1, Usart_Data);

		//判断是否发送完成
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	}

}

void RCC_Configuration(void)
{
	//使能 USART1 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						RCC_APB2Periph_USART1,ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART_Configuration(void)
{
	USART_InitTypeDef 	USART_InitStructure;

	USART_InitStructure.USART_BaudRate 				= 115200;   						//波特率设置;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//一个停止位
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;	//硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//收发模式，收发模式
	
	//初始化串口
	USART_Init(USART1, &USART_InitStructure); 
	
	//使能串口
	USART_Cmd(USART1, ENABLE); 
}

//串口1初始化
void Usart1_Init(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	NVIC_Configuration();
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	//USART_IT_RXNE：接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	//响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;  
	//IRQ 通道使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	//根据指定的参数初始化 VIC 寄存器
	NVIC_Init(&NVIC_InitStructure);  
}
