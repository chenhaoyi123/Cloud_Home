#include "esp8266.h"

/************************************************
 * 重启ESP8266模块
 * 电平先低后高，对ESP8266进行GPIO16引脚复位重启
 * 或使用命令：AT+RST\r\n
*************************************************/
bool ESP8266_Rst(void)
{
	char str[20];

	sprintf(str, "AT+RST\r\n");
	
	return  ESP8266_Send_AT_Cmd(str, "ready", NULL, 5000);
}

/******************************************
 *	对ESP8266模块发送AT指令
 *	cmd：待发送的指令
 *	response：期待的响应(返回：成功)，为NULL表示不需要表示
 *	error：错误的返回值
 *	time：等待响应的时间
 *	返回值：0、发送失败；1、发送成功
 ******************************************/
bool ESP8266_Send_AT_Cmd(char *cmd, char *response, char *error, u32 waittime)
{
	printf("%s\n", cmd);
	Usart2_Send_String(cmd, strlen(cmd));//AT命令从串口2向8266发送

	if(response == NULL && error == NULL)	 //不需要接收数据
		return true;
	
	delay_ms(waittime);	  //延时time时间
	printf("ESP8266_Send_AT_Cmd[%d]：%s\n\n", __LINE__, rx_buffer);//向串口1（电脑）发送数据

	if(response != NULL && error != NULL)
		return ((bool)strstr((char *)rx_buffer, response ) || (bool)strstr((char *)rx_buffer, error));
		
	else if(response != 0)
		return ((bool)strstr((char *)rx_buffer, response));
		
	else
		return ((bool)strstr((char *)rx_buffer, error));
}

/****************************************************************
 *	选择ESP8266模块的工作模式
 *	Mode：工作模式(1.Station模式、2.AP模式、3.AP兼Station模式)
*	返回：1.选择成功 
		  0.选择失败
*****************************************************************/
bool ESP8266_Net_Mode_Choose(Net_ModeTypeDef Mode)
{
	switch(Mode)
	{
		case STA:
			return ESP8266_Send_AT_Cmd("AT+CWMODE=1\r\n", "OK", "no change", 500);
		case AP:
			return ESP8266_Send_AT_Cmd("AT+CWMODE=2\r\n", "OK", "no change", 500);
		case STA_AP:
			return ESP8266_Send_AT_Cmd("AT+CWMODE=3\r\n", "OK", "no change", 500);
		default:
			return false;
	}
}

/************************************
 *	ESP8266模块连接外部WiFi
 *	输入：SSID：WiFi名称字符串
		  pPassWord：WiFi密码字符串
 *	返回1：连接成功 0：连接失败
*************************************/
bool ESP8266_JoinAP(char *SSID, char *PassWord)
{
	char Cmd[120];

	sprintf(Cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PassWord);
	
	return ESP8266_Send_AT_Cmd(Cmd, "OK", NULL, 5000);
}

/********************************************************************
 *	ESP8266创建WiFi热点
 *	输入：name WiFi名称字符串
		  pwd WiFi密码字符串
		  通道号:11、
		  加密方式：(0.OPEN、1.WEP、2.WPA_PSK、3.WPA2_PSK、4.WPA_WPA2_PSK)
*********************************************************************/
bool ESP8266_BuildAP(char *name, char *pwd, u8 num, u8 type)
{
	char Cmd[50];
	sprintf(Cmd, "AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n", name, pwd, num, type);
	
	return ESP8266_Send_AT_Cmd(Cmd, "OK", NULL, 500);
}

/*************************************
 *	ESP8266模块设置连接模式(0.单路连接模式,1.多路连接模式)
 *	输入：ENABLE->1; DISABLE->0
 *	返回：1.配置成功 
		  0.配置失败
**************************************/
bool ESP8266_Enable_MultipleId(FunctionalState NewState)
{
	char Str[20];
	sprintf(Str, "AT+CIPMUX=%d\r\n", (NewState ? 1 : 0));
	
	return ESP8266_Send_AT_Cmd(Str, "OK", NULL, 500);
}

//设置IP地址
bool ESP8266_IP(char *ip)
{
	char str[20];
	sprintf(str, "AT+CIPAP=\"%s\"\r\n", ip);
	
	return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
}

/********************
 *	设置为透传模式
 */
bool ESP8266_Unvarnish_Send(u8 MODE)
{
	char str[20];
	switch(MODE)
	{
		//非透传模式
		case 0:
			sprintf(str, "AT+CIPMODE=%d\r\n", MODE);
			return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
		//透传模式
		case 1:
			sprintf(str, "AT+CIPMODE=%d\r\n", MODE);
			return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
		default:
			return false;
	}
}

/********************************************
 *	ESP8266模块连接外部服务器(建立TCP/UDP连接)
 *	protocol：	网络协议
 *	ip：		服务器IP字符串
 *	ComNum：	服务器端口字符串
 *	返回：		1.连接成功 
				0.连接失败
*********************************************/
bool ESP8266_Link_Server(NetPro_TypeDef protocol, char *ip, u32 PortNum)
{
	char Str[100] = {0}, Cmd[120];

  	switch(protocol)
  	{
		case TCP:
		  sprintf(Str, "\"%s\",\"%s\",%d\r\n", "TCP", ip, PortNum);
		  break;
		
		case UDP:
		  sprintf(Str, "\"%s\",\"%s\",%d\r\n", "UDP", ip, PortNum);
		  break;
		
		default:
			break;
  	}

	sprintf(Cmd, "AT+CIPSTART=%s", Str );
	return ESP8266_Send_AT_Cmd (Cmd, "OK", "ALREAY CONNECT", 4000);
}

/******************************************************
 * 描述  ：ESP8266模块开启或关闭服务器模式
 * 输入  ：ENABLE->1; DISABLE->0
 *       ：PortNum，服务器端口号字符串
 * 返回  ：1.操作成功
 *         0.操作失败
 * 调用  ：被外部调用
*******************************************************/
bool ESP8266_ServerStatus(FunctionalState NewState, u32 PortNum, u32 overtime)
{
	char Cmd1[120], Cmd2[120];

	if(NewState)
	{
		sprintf(Cmd1, "AT+CIPSERVER=%d,%d\r\n", 1, PortNum);
		sprintf(Cmd2, "AT+CIPSTO=%d", overtime);

		return (ESP8266_Send_AT_Cmd(Cmd1, "OK", NULL, 500) && 
					ESP8266_Send_AT_Cmd(Cmd2, "OK", NULL, 500));
	}
	else
	{
		sprintf(Cmd1, "AT+CIPSERVER=%d,%d\r\n", 0, PortNum);

		return ESP8266_Send_AT_Cmd(Cmd1, "OK", NULL, 500);
	}
}

/********************************************************
 * 描述  ：获取ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，获得ip
 *         3，建立连接
 *         3，失去连接
 *         0，获取状态失败
 * 调用  ：被外部调用
*********************************************************/
uint8_t ESP8266_Get_LinkStatus(void)
{
	if(ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", NULL, 500 ))
	{
		if(strstr((char *)rx_buffer, "STATUS:2\r\n" ) )
			return 2;
		
		else if(strstr((char *)rx_buffer, "STATUS:3\r\n" ) )
			return 3;
		
		else if(strstr((char *)rx_buffer, "STATUS:4\r\n" ) )
			return 4;		
	}
	return 0;
}

/******************************************************************
 * 函数名：ESP8266_Get_IdLinkStatus
 * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
 * 输入  ：无
 * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，
		   某位若置1表该Id建立了连接，若被清0表该Id未建立连接
 * 调用  ：被外部调用
********************************************************************/
uint8_t ESP8266_Get_IdLinkStatus(void)
{
	uint8_t ucIdLinkStatus = 0x00;
	
	if(ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", NULL, 500 ) )
	{
		if(strstr((char *)rx_buffer, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if(strstr((char *)rx_buffer, "+CIPSTATUS:1," ))
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if(strstr((char *)rx_buffer, "+CIPSTATUS:2," ))
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if(strstr((char *)rx_buffer, "+CIPSTATUS:3," ))
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if(strstr((char *)rx_buffer, "+CIPSTATUS:4," ))
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	
	return ucIdLinkStatus;	
}

/*************************************************
 *	ESP8266模块发送字符串
 *	输入：ENABLE->1; DISABLE->0
 *	Mode：声明是否已使能了透传模式
 *	Str：要发送的字符串
 *	Id：向哪个ID发送字符串
 *	返回1：发送成功 0：发送失败
**************************************************/
bool ESP8266_SendString(FunctionalState NewState, char *Str, ID_Num_TypeDef Id)
{
	u32 Len;
	char Send_Str[500];
	
	if(NewState)
	{
		Len = strlen(Str);
		if(Id < 5)
			sprintf(Send_Str, "AT+CIPSEND=%d,%d\r\n", Id, Len);
		else
			sprintf(Send_Str, "AT+CIPSEND\r\n");

		ESP8266_Send_AT_Cmd(Send_Str, "> ", NULL, 1000);

		return ESP8266_Send_AT_Cmd(Str, "SEND OK", NULL, 1000);
	}
	else
	{
		Usart2_Send_String(Str, strlen(Str));
		
		return true;
	}
	
	
}

/***********************************
 *	配置ESP8266模块进入透传发送数据
 *	返回1：配置成功 0：配置失败
************************************/
bool ESP8266_UnvarnishSend(void)
{
	if (!ESP8266_Send_AT_Cmd("AT+CIPMODE=1\r\n", "OK", NULL, 500))
		return false;
	
	return 
	  	ESP8266_Send_AT_Cmd("AT+CIPSEND\r\n", "OK", ">", 500 );
}

/********************************************
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
**********************************************/
void ESP8266_ExitUnvarnishSend(void)
{
	delay_ms(1000);
	
	Usart2_Send_String("+++", strlen("+++"));
	
	delay_ms(500); 
	
}

/******************
 *	接收数据字符比较
********************/
bool ESP8266_Compare(char *recdata)
{
 	strstr((char *)rx_buffer, recdata);
	
	return 0;
}

