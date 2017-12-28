#include "usart3.h"
#include "stdio.h"

u8	USART3_RX_FLAG;				//1��ʾ���ڽ��������� 0������
u8 	USART3_RX_BUF[USART3_MAX_RECV_LEN]; 			//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u16 USART3_RX_STA = 0; //��������״̬


//����3�жϷ������
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	//���յ�����
	{	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);

		if(USART3_RX_STA < USART3_MAX_RECV_LEN)
		{
			/* ��ȡ���ܵ������ݲ����浽buffer������*/
			USART3_RX_BUF[USART3_RX_STA++] = USART_ReceiveData(USART3);
		}
		else 
			USART3_RX_STA |= 1<<15;	//ǿ�Ʊ�ǽ������
	
//		//������3���ж����ݷ��͵�����1��
//		USART_SendData(USART1, USART_ReceiveData(USART3));

//		//�жϴ���3�Ƿ������
//		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
	}
}

//����2��ʼ��
void Usart3_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	
	//ʹ�� USART3��IO�� ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	
	
	//����Ϊ�����������	USART3_TX-->PD8,PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//����Ϊ��������	USART3_RX-->PD9, PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate 		= BaudRate; 			//����������;  
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽʹ��
	//��ʼ������
	USART_Init(USART3, &USART_InitStructure);  
	//ʹ��USART����
	USART_Cmd(USART3, ENABLE);
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//��Ӧ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 	//����ָ���Ĳ�����ʼ�� VIC �Ĵ���
	
	//ʹ�ܴ���2�����ж�
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
