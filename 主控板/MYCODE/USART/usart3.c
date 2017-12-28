#include "usart3.h"
#include "stdio.h"

u8	USART3_RX_FLAG;				//1表示串口接收完数据 0无数据
u8 	USART3_RX_BUF[USART3_MAX_RECV_LEN]; 			//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
u16 USART3_RX_STA = 0; //接收数据状态


//串口3中断服务程序
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//接收到数据
	{	
		//判断为真后，为下次中断做准备，则需要对中断的标志清零
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);

		if(USART3_RX_STA < USART3_MAX_RECV_LEN)
		{
			/* 读取接受到的数据并保存到buffer数组中*/
			USART3_RX_BUF[USART3_RX_STA++] = USART_ReceiveData(USART3);
		}
		else 
			USART3_RX_STA |= 1<<15;	//强制标记接收完成
	
//		//将串口3的中断数据发送到串口1中
//		USART_SendData(USART1, USART_ReceiveData(USART3));

//		//判断串口3是否发送完成
//		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
	}
}

//串口2初始化
void Usart3_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	
	//使能 USART3和IO口 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	
	
	//设置为复用推挽输出	USART3_TX-->PD8,PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//设置为浮空输入	USART3_RX-->PD9, PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate 		= BaudRate; 			//波特率设置;  
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//一个停止位
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式使能
	//初始化串口
	USART_Init(USART3, &USART_InitStructure);  
	//使能USART外设
	USART_Cmd(USART3, ENABLE);
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); 	//根据指定的参数初始化 VIC 寄存器
	
	//使能串口2接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void Usart3_Send_Byte(unsigned char SendData)
{
	USART_SendData(USART3, SendData);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Usart3_Send_String(u8 *Data, u16 len)
{
	u16 i = 0;
	while((*(Data+i) != 0) && (i < len))
	{
		Usart3_Send_Byte(*(Data+i));
		i++;
	}
}
