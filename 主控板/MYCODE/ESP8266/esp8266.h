#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "stm32f10x.h"
#include <stdbool.h>
#include <stdio.h>
#include "usart2.h"
#include "sys.h"
#include "delay.h"
#include "esp8266_Config.h"

//ESP8266数据类型定义
typedef enum
{
	STA,
  	AP,
  	STA_AP  
}Net_ModeTypeDef;

//ESP8266网络连接类型定义
typedef enum{
	 TCP,
	 UDP,
}NetPro_TypeDef;

//ESP8266发送ID号定义(最多可连接5个客户端)
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
}ID_Num_TypeDef;

bool ESP8266_Rst(void);
bool ESP8266_Send_AT_Cmd(char *cmd, char *response, char *error, u32 waittime);
bool ESP8266_Net_Mode_Choose(Net_ModeTypeDef Mode);
bool ESP8266_JoinAP(char *SSID, char *PassWord);
bool ESP8266_BuildAP(char *name, char *pwd, u8 num, u8 type);
bool ESP8266_Enable_MultipleId(FunctionalState NewState);
bool ESP8266_IP(char *ip);
bool ESP8266_Unvarnish_Send(u8 MODE);
bool ESP8266_Link_Server(NetPro_TypeDef protocol, char *ip, u32 PortNum);
bool ESP8266_ServerStatus(FunctionalState NewState, u32 PortNum, u32 overtime);
uint8_t ESP8266_Get_LinkStatus(void);
uint8_t ESP8266_Get_IdLinkStatus(void);
bool ESP8266_SendString(FunctionalState NewState, char *Str, ID_Num_TypeDef Id);
bool ESP8266_UnvarnishSend(void);
bool ESP8266_Compare(char *recdata);
void ESP8266_ExitUnvarnishSend(void);

#endif
