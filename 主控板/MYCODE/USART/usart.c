#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	//�򴮿�1��������
	USART_SendData(USART1, (u8)ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	
	return ch;
}

//����1�жϷ������
void USART1_IRQHandler(void)
{
	u8 Usart_Data;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//���յ�����
	{	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		/* DR��ȡ���յ������� */
		Usart_Data = USART_ReceiveData(USART1);

		//�յ����ݾ�ֱ�ӷ��͵�����1
		USART_SendData(USART1, Usart_Data);

		//�ж��Ƿ������
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	}

}

void RCC_Configuration(void)
{
	//ʹ�� USART1 ʱ��
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

	USART_InitStructure.USART_BaudRate 				= 115200;   						//����������;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;				//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;					//һ��ֹͣλ
	USART_InitStructure.USART_Parity 				= USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ���շ�ģʽ
	
	//��ʼ������
	USART_Init(USART1, &USART_InitStructure); 
	
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE); 
}

//����1��ʼ��
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
	
	//USART_IT_RXNE�������ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	//��Ӧ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;  
	//IRQ ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	//����ָ���Ĳ�����ʼ�� VIC �Ĵ���
	NVIC_Init(&NVIC_InitStructure);  
}
