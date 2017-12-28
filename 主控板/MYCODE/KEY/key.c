#include "key.h"
#include "delay.h"

//S1->PE2, S2->PE3, S3->PE4
void Key_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	
	//��ʼ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;					//��������
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//����ɨ�躯��
u8 Key_Scan(void)
{
	//�жϰ���KEY0�Ƿ���
	if(KEY0 == 0)  //0�ǰ���,��ͼʾ�����͵�ƽΪ����
	{
		delay_ms(10);	//����
		if(KEY0 == 0)
			return 0;  //�ɹ����£��򷵻�0
	}
	
	//�жϰ���KEY1�Ƿ���
	if(KEY1 == 0)  //0�ǰ���,��ͼʾ����
	{
		delay_ms(10);
		if(KEY1 == 0)
			return 1;
	}
	
	//�жϰ���KEY2�Ƿ���
	if(KEY2 == 0)  //0�ǰ���,��ͼʾ����
	{
		delay_ms(10);
		if(KEY2 == 0)
			return 2;
	}
	
	return 3;
}
