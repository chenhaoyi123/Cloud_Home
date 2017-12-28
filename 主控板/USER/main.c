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

const u8 IP1[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//dht11+bh1750��С���ַ
const u8 IP2[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x02};	//sg90�����С���ַ
const u8 IP3[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x03};	//�������
const u8 IP4[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x04};	//����

int id; //�����趨С���IP��ַ
int time_out = 0;
u8 tx_buff[25];
u8 rx_buff[25];

int main(void)
{
	Led_Init();			//LED�Ƴ�ʼ��
	Beep_Init();		//��������ʼ��
	Delay_init();		//��ʱ������ʼ��
	Key_Init();			//������ʼ��
  Usart1_Init();	//����1��ʼ��
	ESP8266_Init();		//WiFiģ���ʼ��
	NRF24L01_Init();	//��ʼ��2.4G
	TIM3_Int_Init();	//��ʼ��tim3��ʱ��
	
	printf("Init OK!\n");
	
  while(1)
	{

		//���ݽ������
		if(USART2_RX_FLAG == 1)
		{
			printf("rx_buffer��%s\n", rx_buffer);
			if(strstr((char *)rx_buffer, "bh17")) //bh17  ���ǿ��     
			{
				sprintf((char *)rx_buffer, "bh17");
				if(nrf24l01_fun((u8 *)IP1)==0)
				{
					ESP8266_SendString(ENABLE, (char *)rx_buff, Single_ID_0);
				}
			}
			if(strstr((char *)rx_buffer, "dhtl"))	//dhtl		����ʪ��
			{
				sprintf((char *)rx_buffer, "dhtl");
				if(nrf24l01_fun((u8 *)IP1)==0)
				{
					ESP8266_SendString(ENABLE, (char *)rx_buff, Single_ID_0);
				}
			}
			if(strstr((char *)rx_buffer, "sg90"))	//sg90  �򿪶��
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
				LED_B_5 = 0;	//�͵�ƽ����
			}
			if(strstr((char *)rx_buffer, "led5c"))
			{
				printf("LED_5_CLOSE\n");
				LED_B_5 = 1;	//�ߵ�ƽ���
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
			USART2_RX_FLAG = 0;	//�������λ
			//memset(rx_buffer, 0, sizeof(rx_buffer));
		}
		
	}
}

//2.4�������ݺ���
int nrf24l01_fun(u8* IP)
{	
		//2.4G���
		while(NRF24L01_Check())
		{
			printf("NRF ERROR!\n");
		}
		while(1)
		{		
			NRF24L01_TX_Mode((u8*)IP);		//����2.4gģ��Ϊ����ģʽ
			delay_ms(5);
			if(NRF24L01_TxPacket(tx_buff)== TX_OK)	//ͨ��2.4g����һ�����ݣ��ɹ�����TX_OK==0x20
			{
				break;
			}
			if(time_out == 15)//15����Ӧ��ǿ�ƽ�������
			{
				time_out = 0;
				return -1;
			}
		}
			
		//2.4G���
		while(NRF24L01_Check())
		{
			printf("NRF ERROR!\n");
		}

		while(1)
		{
			NRF24L01_RX_Mode((u8*)IP);		//����2.4gģ��Ϊ����ģʽ
			delay_ms(5);
			if(NRF24L01_RxPacket(rx_buff) == 0)		//ͨ��2.4g����һ�����ݣ��ɹ�����0
			{
				break;
			}
			if(time_out == 15)//15����Ӧ��ǿ�ƽ�������
			{
				time_out = 0;
				return -1;
			}
		}
		return 0;
}
