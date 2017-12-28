#include "dht11.h"

void DHT11_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);						//使能GPIOA时钟

	
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP; 						//推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, & GPIO_InitStructure);													//初始化GPIOA
}


void DHT11_int(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	GPIO_InitStructure.GPIO_Pin  	= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;					//浮空输入
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, & GPIO_InitStructure);
}

int DHT11_Start(void)
{
	u8 i = 0;
	
	DHT11_out();											//先将PA3口设为输出
	PAout(3) = 0;											//输出低电平
	delay_ms(20);											//延迟20毫秒
	
	PAout(3) = 1;											//输出高电平
	delay_us(20);											//延时20微秒
	
	DHT11_int();											//将PA3口设为输入
	
	while(i<100)											//超时检测，检测低电平																	
	{
		if(PAin(3) == 0)
		{
			break;
		}
		delay_us(1);
		i++;
	}
	
	if(PAin(3) == 1)									//失败返回1
	{
		return 1;
	}
	else{
		i = 0;
	}	
		
	
	delay_us(70);											//延时
	
	while(i<100)											//超时检测，检测高电平
	{
		if(PAin(3) == 1)							
		{
			break;
		}	
		delay_us(1);
		i++;
	}
	if(PAin(3) == 1)										//正确返回0
	{
		return 0;
	}
	else{
		return 1;
	}
		
}

u8 DHT11_Pooled_Data(void)						//读到数据后将八位数据合并成一个字节
{
	u8 i,data=0;
	
	for(i=0; i<8; i++)
	{
		
		while(PAin(3) == 0);							//等待高电平
		
		
		delay_us(40);											//延时40微秒
		
		if(PAin(3) == 1)									//延时后判断高电平为1，
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

	while(PAin(3) == 1);							//等到高电平
	
	for(i=0; i<5; i++)
	{
		data[i] = DHT11_Pooled_Data();	//读取数据
	}

	if(data[4] ==  (data[0] +data[1] + data[2] + data[3]))
	{
		return 0;												//数据校准
	}

	return 1;
}

int DHT11_Start_Sensor(u8 *data)
{
	while(DHT11_Start() == 1);				//启动DHT11
	
	if(DHT11_Read_Data(data) == 1)
	{
		return 1;
	}
	
	return 0;
}


	
