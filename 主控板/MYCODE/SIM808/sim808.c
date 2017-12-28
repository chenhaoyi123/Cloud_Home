#include "sim808.h"
#include "string.h"
#include "stdio.h"
#include "usart3.h"
#include "delay.h"

u8 SIM900_CSQ[3];
u8 dtbuf[50];   //��ӡ������	

u8* sim900a_check_cmd(u8 *str)
{
	char *strx = 0;
	if(USART3_RX_STA & 0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0;//��ӽ�����
		strx = strstr((const char*)USART3_RX_BUF, (const char*)str);
	} 
	return (u8*)strx;
}

u8 sim900a_send_cmd(u8 *cmd, u8 *ack, u16 waittime)
{
	u8 res = 0; 
	USART3_RX_STA = 0;
//	if((u32)cmd <= 0XFF)
//	{
//		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
//		USART2->DR=(u32)cmd;
//	}
//	else 
		Usart3_Send_String(cmd, strlen((char *)cmd));//��������
	if(ack && waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA & 0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))
					break;//�õ���Ч���� 
				USART3_RX_STA = 0;
			} 
		}
		if(waittime == 0)
			res = 1; 
	}
	return res;
}

u8 sim900a_work_test(void)
{
	if(sim900a_send_cmd((u8 *)"AT", (u8 *)"OK", 100))
	{
		if(sim900a_send_cmd((u8 *)"AT", (u8 *)"OK", 100))
			return SIM_COMMUNTION_ERR;	//ͨ�Ų���
	}		
	if(sim900a_send_cmd((u8 *)"AT+CPIN?", (u8 *)"READY", 400))
		return SIM_CPIN_ERR;	//û��SIM��
	if(sim900a_send_cmd((u8 *)"AT+CREG?", (u8 *)"0,1", 400))
	{
		if(strstr((const char*)USART3_RX_BUF, "0,5") == NULL)
		{
			 if(!sim900a_send_cmd((u8 *)"AT+CSQ", (u8 *)"OK", 200))	
			 {
					memcpy(SIM900_CSQ, USART3_RX_BUF+15, 2);
			 }
			 return SIM_CREG_FAIL;	//�ȴ����ŵ�����
		}
	}
	return SIM_OK;
}

//��ʼ��
u8 SIM_Init(void)
{
	u8 ret;
	
	Usart3_Init(115200);	//��ʼ������3
	
	ret = sim900a_work_test();
	switch(ret)
	{
		case SIM_OK:
			printf("GSMģ���Լ�ɹ�\n");
			break;
		case SIM_COMMUNTION_ERR:
			printf("��GSMģ��δͨѶ�ɹ�����ȴ�\n");
			break;
		case SIM_CPIN_ERR:
			printf("û��⵽SIM��\n");
			break;
		case SIM_CREG_FAIL:
			printf("ע��������......\n");
			printf("��ǰ�ź�ֵ��%s\n", SIM900_CSQ);
			break;
		default:
			break;
	}
	
	return ret;
}
