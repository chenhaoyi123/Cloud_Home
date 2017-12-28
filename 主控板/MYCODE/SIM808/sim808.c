#include "sim808.h"
#include "string.h"
#include "stdio.h"
#include "usart3.h"
#include "delay.h"

u8 SIM900_CSQ[3];
u8 dtbuf[50];   //打印缓存器	

u8* sim900a_check_cmd(u8 *str)
{
	char *strx = 0;
	if(USART3_RX_STA & 0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA & 0X7FFF] = 0;//添加结束符
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
//		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
//		USART2->DR=(u32)cmd;
//	}
//	else 
		Usart3_Send_String(cmd, strlen((char *)cmd));//发送命令
	if(ack && waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA & 0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))
					break;//得到有效数据 
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
			return SIM_COMMUNTION_ERR;	//通信不上
	}		
	if(sim900a_send_cmd((u8 *)"AT+CPIN?", (u8 *)"READY", 400))
		return SIM_CPIN_ERR;	//没有SIM卡
	if(sim900a_send_cmd((u8 *)"AT+CREG?", (u8 *)"0,1", 400))
	{
		if(strstr((const char*)USART3_RX_BUF, "0,5") == NULL)
		{
			 if(!sim900a_send_cmd((u8 *)"AT+CSQ", (u8 *)"OK", 200))	
			 {
					memcpy(SIM900_CSQ, USART3_RX_BUF+15, 2);
			 }
			 return SIM_CREG_FAIL;	//等待附着到网络
		}
	}
	return SIM_OK;
}

//初始化
u8 SIM_Init(void)
{
	u8 ret;
	
	Usart3_Init(115200);	//初始化串口3
	
	ret = sim900a_work_test();
	switch(ret)
	{
		case SIM_OK:
			printf("GSM模块自检成功\n");
			break;
		case SIM_COMMUNTION_ERR:
			printf("与GSM模块未通讯成功，请等待\n");
			break;
		case SIM_CPIN_ERR:
			printf("没检测到SIM卡\n");
			break;
		case SIM_CREG_FAIL:
			printf("注册网络中......\n");
			printf("当前信号值：%s\n", SIM900_CSQ);
			break;
		default:
			break;
	}
	
	return ret;
}
