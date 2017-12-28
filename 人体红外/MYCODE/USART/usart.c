#include "usart.h"
#include "stdio.h"

void Usart1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	//ʹ�õ��Ǵ��� 1������ 1 ��Ӧ��оƬ���� PA9,PA10 ��Ҫʹ��PA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            
  GPIO_Init(GPIOA, &GPIO_InitStructure);     
	
	
	
	//����������;
	USART_InitStructure.USART_BaudRate 		= 115200;   
	//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	//һ��ֹͣλ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	//����żУ��λ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	//Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//�շ�ģʽ���շ�ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//��ʼ������
	USART_Init(USART1, &USART_InitStructure); 
	
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE); 

	//���������ж�
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//��Ӧ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;  
	//IRQ ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	//����ָ���Ĳ�����ʼ�� VIC �Ĵ���
	NVIC_Init(&NVIC_InitStructure);  

}
