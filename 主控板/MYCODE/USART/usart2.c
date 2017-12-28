#include "usart2.h"
#include "stdio.h"
#include "string.h"

u8 	USART2_RX_BUF[USART2_MAX_RECV_LEN]; 			//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8	USART2_RX_FLAG;				//1��ʾ���ڽ��������� 0������
u8	rx_count = 0;  //ͳ�ƽ��ո���
u8  rx_buffer[500] = {0};

//����2�жϷ������
void USART2_IRQHandler(void)
{
	u8 rx_i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	
		//�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		/* ��ȡ���ܵ������ݲ����浽������*/
		USART2_RX_BUF[rx_count++] = USART_ReceiveData(USART2);	//�����ж�����
		
		//�ж��Ƿ���ս���
		if(USART2_RX_BUF[rx_count-1]  == 0x0A || USART2_RX_BUF[rx_count-1]  == '*')
		{
			memset(rx_buffer, 0, sizeof(rx_buffer));
			for(rx_i = 0; rx_i < rx_count; rx_i++)
			{
				rx_buffer[rx_i] = USART2_RX_BUF[rx_i];
			}
			//��ռ���
			rx_count = 0;
			//���buffer
			memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));

			USART2_RX_FLAG  = 1;  //��ʾ�յ�����
		}
	}
}

//����2��ʼ��
void Usart2_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;		//����ܽŽṹ�����
	USART_InitTypeDef 	USART_InitStructure;	//���崮�ڽṹ�����
	NVIC_InitTypeDef 	NVIC_InitStructure;		//�ж����ȼ��ṹ�����
	
	//ʹ�� USART2 ʱ�Ӻ͹ܽ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	
	//����Ϊ�����������	USART2_TX-->PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����Ϊ��������	USART2_RX-->PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate 		= BaudRate; 			//����������;  
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;	//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;		//һ��ֹͣλ
	USART_InitStructure.USART_Parity 		= USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽʹ��
	//��ʼ������
	USART_Init(USART2, &USART_InitStructure);  
	
	//ʹ�ܴ���2����(RXNE)�Ϳ�������(IDLE)�ж�
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//USART2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//��Ӧ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); 	//����ָ���Ĳ�����ʼ�� NVIC �Ĵ���

	USART_Cmd(USART2, ENABLE);//����2ʹ��
}

//����2�����ַ�
void Usart2_Send_Byte(unsigned char SendData)
{
	USART_SendData(USART2, SendData);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET);
}

//����2�����ַ���
void Usart2_Send_String(char *Data, u16 len)
{
	u16 i = 0;
	while((*(Data+i) != NULL) && (i <= len))
	{
		Usart2_Send_Byte(*(Data+i));
		i++;
	}
}
