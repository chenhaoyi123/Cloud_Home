#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "nRF24L01_API.h"
#include "sg90_pwm.h"
#include "BH1750.h"
#include "i2c.h"
#include "dht11.h"
#include "tim3.h"

int time_out = 0;
float result_lx;
u8 data[5]				= {0};

const u8 IP1[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//dht11+bh1750的小板地址
const u8 IP2[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x02};	//sg90舵机与rfid的小板地址
const u8 IP3[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x03};	//步进电机
const u8 IP4[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x04};	//待定

int main(void)
{
		u8 tx_buff[25];
		u8 rx_buff[25];
		
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		Led_Init();
		Systick_Init();
		Usart1_Init();
		NRF24L01_Init();
		SG90_pwm_init();
		Init_BH1750();
		
		PCout(13)=0;
		delay_ms(100);
		PCout(13)=1;
		delay_ms(100);
		PCout(13)=0;
		delay_ms(100);
		PCout(13)=1;
		
		//2.4G接收端
		while(NRF24L01_Check()){}

		while(1)
		{
			while(1)
			{
				NRF24L01_RX_Mode((u8*)IP1);
				delay_ms(5);
				if(NRF24L01_RxPacket(tx_buff) == 0)
				{
					break;
				}
			}
			
		
			if(strncmp((char*)tx_buff,"bh17",4) == 0)
			{
				Start_BH1750();  		//power on
				delay_ms(180);       //延时180ms
				Read_BH1750();     	
				delay_ms(180);       //延时180ms
				Convert_BH1750(); 		//结果保存在result_lx
				sprintf((char *)rx_buff, "光强度:%.2f\n", result_lx);
			}
			if(strncmp((char*)tx_buff,"dhtl",4) == 0)
			{
				if(DHT11_Start_Sensor(data) == 0)
				{
					sprintf((char *)rx_buff, "湿度:%d.%d\t温度:%d.%d\r\n", data[0], data[1], 
																data[2], data[3]);
				}
			}	
			while(1)
			{
				NRF24L01_TX_Mode((u8*)IP1);
				delay_ms(5);
				if(NRF24L01_TxPacket(rx_buff)== TX_OK)
				{
					break;
				}
				if(time_out == 20)
				{
					time_out = 0;
					break;
				}
			}
		}
	
}
