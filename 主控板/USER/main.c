#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "beep.h"
#include "key.h"
#include "usart.h"
#include "esp8266.h"
#include "delay.h"
#include "nRF24L01_API.h"
#include "tim3.h"

int nrf24l01_fun(u8* IP);

const u8 IP1[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//dht11+bh1750的小板地址
const u8 IP2[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x02};	//sg90舵机的小板地址
const u8 IP3[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x03};	//步进电机
const u8 IP4[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x04};	//待定

int id; //用于设定小板的IP地址
int time_out = 0;
u8 tx_buff[25];
u8 rx_buff[25];

int main(void)
{
	Led_Init();			//LED灯初始化
	Beep_Init();		//蜂鸣器初始化
	Delay_init();		//延时函数初始化
	Key_Init();			//按键初始化
  Usart1_Init();	//串口1初始化
	ESP8266_Init();		//WiFi模块初始化
	NRF24L01_Init();	//初始化2.4G
	TIM3_Int_Init();	//初始化tim3定时器
	
	printf("Init OK!\n");
	
  while(1)
	{

		//数据接收完成
		if(USART2_RX_FLAG == 1)
		{
			printf("rx_buffer：%s\n", rx_buffer);
			if(strstr((char *)rx_buffer, "bh17")) //bh17  测光强度     
			{
				sprintf((char *)rx_buffer, "bh17");
				if(nrf24l01_fun((u8 *)IP1)==0)
				{
					ESP8266_SendString(ENABLE, (char *)rx_buff, Single_ID_0);
				}
			}
			if(strstr((char *)rx_buffer, "dhtl"))	//dhtl		测温湿度
			{
				sprintf((char *)rx_buffer, "dhtl");
				if(nrf24l01_fun((u8 *)IP1)==0)
				{
					ESP8266_SendString(ENABLE, (char *)rx_buff, Single_ID_0);
				}
			}
			if(strstr((char *)rx_buffer, "sg90"))	//sg90  打开舵机
			{
				sprintf((char *)rx_buffer, "sg90");
				if(nrf24l01_fun((u8 *)IP2)==0)
				{
					ESP8266_SendString(ENABLE, (char *)rx_buff, Single_ID_0);
				}
			}
			if(strstr((char *)rx_buffer, "led5o"))
			{
				printf("LED_5_OPEN\n");
				LED_B_5 = 0;	//低电平亮灯
			}
			if(strstr((char *)rx_buffer, "led5c"))
			{
				printf("LED_5_CLOSE\n");
				LED_B_5 = 1;	//高电平灭灯
			}
			if(strstr((char *)rx_buffer, "led6o"))
			{
				printf("LED_6_OPEN\n");
				LED_E_6 = 0;
			}
			if(strstr((char *)rx_buffer, "led6c"))
			{
				printf("LED_6_CLOSE\n");
				LED_E_6 = !LED_E_6;
			}
			USART2_RX_FLAG = 0;	//清空数据位
			//memset(rx_buffer, 0, sizeof(rx_buffer));
		}
		
	}
}

//2.4发送数据函数
int nrf24l01_fun(u8* IP)
{	
		//2.4G检查
		while(NRF24L01_Check())
		{
			printf("NRF ERROR!\n");
		}
		while(1)
		{		
			NRF24L01_TX_Mode((u8*)IP);		//设置2.4g模块为发送模式
			delay_ms(5);
			if(NRF24L01_TxPacket(tx_buff)== TX_OK)	//通过2.4g发送一次数据，成功返回TX_OK==0x20
			{
				break;
			}
			if(time_out == 15)//15秒无应答强制结束发送
			{
				time_out = 0;
				return -1;
			}
		}
			
		//2.4G检查
		while(NRF24L01_Check())
		{
			printf("NRF ERROR!\n");
		}

		while(1)
		{
			NRF24L01_RX_Mode((u8*)IP);		//设置2.4g模块为接收模式
			delay_ms(5);
			if(NRF24L01_RxPacket(rx_buff) == 0)		//通过2.4g接收一次数据，成功返回0
			{
				break;
			}
			if(time_out == 15)//15秒无应答强制结束接收
			{
				time_out = 0;
				return -1;
			}
		}
		return 0;
}
