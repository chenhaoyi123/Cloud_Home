#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "nRF24L01_API.h"
#include "sg90_pwm.h"
#include "tim3.h"

int time_out = 0;

const u8 IP1[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//dht11+bh1750的小板地址
const u8 IP2[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x02};	//sg90舵机与rfid的小板地址
const u8 IP3[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x03};	//步进电机
const u8 IP4[RX_ADR_WIDTH]={0x43,0x10,0x01,0x01,0x04};	//待定

int main(void)
{

		u8 tx_buff[25];
		u8 rx_buff[25] = "door open";
	
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		Led_Init();
		Systick_Init();
		Usart1_Init();
		NRF24L01_Init();
		SG90_pwm_init();
		TIM3_Int_Init();
		
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
				NRF24L01_RX_Mode((u8*)IP2);
				delay_ms(5);
				if(NRF24L01_RxPacket(tx_buff) == 0)
				{
					break;
				}
			}
			
			if(strncmp((char*)tx_buff,"sg90",4) == 0)
			{
				TIM_SetCompare4(TIM4,6);    //通道x，TIM_SetComparex(TIMy,6-25)逆时针转到尽头 0度
				delay_ms(1000);							
				TIM_SetCompare4(TIM4,15);    //通道x，TIM_SetComparex顺时针转到尽头 180度
				delay_ms(3000);
				TIM_SetCompare4(TIM4,6);    //通道x，TIM_SetComparex(TIMy,6-25)逆时针转到尽头 0度
			}
			
	
			
			while(1)
			{
				NRF24L01_TX_Mode((u8*)IP2);	
				delay_ms(5);
				if(NRF24L01_TxPacket(rx_buff)== TX_OK)
				{
					break;
				}
				if(time_out > 10)
				{			
					PCout(13)=0;
					delay_ms(500);
					PCout(13)=1;
					delay_ms(500);
					PCout(13)=0;
					delay_ms(500);
					PCout(13)=1;
					time_out = 0;
					break;
				}
			}

		}
		
	
}
