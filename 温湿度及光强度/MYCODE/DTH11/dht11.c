#include "dht11.h"

void DHT11_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//ʹ��GPIOAʱ��

	
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP; 						//�������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, & GPIO_InitStructure);													//��ʼ��GPIOA
}


void DHT11_int(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;					//��������
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, & GPIO_InitStructure);
}

int DHT11_Start(void)
{
	u8 i = 0;
	
	DHT11_out();											//�Ƚ�PA3����Ϊ���
	PAout(3) = 0;											//����͵�ƽ
	delay_ms(20);											//�ӳ�20����
	
	PAout(3) = 1;											//����ߵ�ƽ
	delay_us(20);											//��ʱ20΢��
	
	DHT11_int();											//��PA3����Ϊ����
	
	while(i<100)											//��ʱ��⣬���͵�ƽ																	
	{
		if(PAin(3) == 0)
		{
			break;
		}
		delay_us(1);
		i++;
	}
	
	if(PAin(3) == 1)									//ʧ�ܷ���1
	{
		return 1;
	}
	else{
		i = 0;
	}	
		
	
	delay_us(70);											//��ʱ
	
	while(i<100)											//��ʱ��⣬���ߵ�ƽ
	{
		if(PAin(3) == 1)							
		{
			break;
		}	
		delay_us(1);
		i++;
	}
	if(PAin(3) == 1)										//��ȷ����0
	{
		return 0;
	}
	else{
		return 1;
	}
		
}

u8 DHT11_Pooled_Data(void)						//�������ݺ󽫰�λ���ݺϲ���һ���ֽ�
{
	u8 i,data=0;
	
	for(i=0; i<8; i++)
	{
		
		while(PAin(3) == 0);							//�ȴ��ߵ�ƽ
		
		
		delay_us(40);											//��ʱ40΢��
		
		if(PAin(3) == 1)									//��ʱ���жϸߵ�ƽΪ1��
		{
			data <<=1;
			data |=1;
			while(PAin(3) == 1); 						
		}
		else
			data <<=1;
	}

	return data;
}


int DHT11_Read_Data(u8 *data)
{
	u8 i=0;

	while(PAin(3) == 1);							//�ȵ��ߵ�ƽ
	
	for(i=0; i<5; i++)
	{
		data[i] = DHT11_Pooled_Data();	//��ȡ����
	}

	if(data[4] ==  (data[0] +data[1] + data[2] + data[3]))
	{
		return 0;												//����У׼
	}

	return 1;
}

int DHT11_Start_Sensor(u8 *data)
{
	while(DHT11_Start() == 1);				//����DHT11
	
	if(DHT11_Read_Data(data) == 1)
	{
		return 1;
	}
	
	return 0;
}


	
