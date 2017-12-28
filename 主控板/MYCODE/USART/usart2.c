#include "usart2.h"
#include "stdio.h"
#include "string.h"

u8 	USART2_RX_BUF[USART2_MAX_RECV_LEN]; 			//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8	USART2_RX_FLAG;				//1表示串口接收完数据 0无数据
u8	rx_count = 0;  //统计接收个数
u8  rx_buffer[500] = {0};

//串口2中断服务程序
void USART2_IRQHandler(void)
{
	u8 rx_i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		/* 读取接受到的数据并保存到数组中*/
		USART2_RX_BUF[rx_count++] = USART_ReceiveData(USART2);	//保存中断数据
		
		//判断是否接收结束
		if(USART2_RX_BUF[rx_count-1]  == 0x0A || USART2_RX_BUF[rx_count-1]  == '*')
		{
			memset(rx_buffer, 0, sizeof(rx_buffer));
			for(rx_i = 0; rx_i < rx_count; rx_i++)
			{
				rx_buffer[rx_i] = USART2_RX_BUF[rx_i];
			}
			//清空计数
			rx_count = 0;
			//清空buffer
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));

			USART2_RX_FLAG  = 1;  //表示收到数据
		}
	}
}

//串口2初始化
void Usart2_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;		//定义管脚结构体变量
	USART_InitTypeDef 	USART_InitStructure;	//定义串口结构体变量
	NVIC_InitTypeDef 	NVIC_InitStructure;		//中断优先级结构体变量
	
	//使能 USART2 时钟和管脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	
	//设置为复用推挽输出	USART2_TX-->PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//设置为浮空输入	USART2_RX-->PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate 		= BaudRate; 			//波特率设置;  
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式使能
	//初始化串口
	USART_Init(USART2, &USART_InitStructure);  
	
	//使能串口2接收(RXNE)和空闲总线(IDLE)中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//USART2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); 	//根据指定的参数初始化 NVIC 寄存器

	USART_Cmd(USART2, ENABLE);//串口2使能
}

//串口2发送字符
void Usart2_Send_Byte(unsigned char SendData)
{
	USART_SendData(USART2, SendData);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
}

//串口2发送字符串
void Usart2_Send_String(char *Data, u16 len)
{
	u16 i = 0;
	while((*(Data+i) != NULL) && (i <= len))
	{
		Usart2_Send_Byte(*(Data+i));
		i++;
	}
}
