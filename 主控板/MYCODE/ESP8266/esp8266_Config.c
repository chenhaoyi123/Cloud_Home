#include "esp8266_Config.h"

//ESP8266初始化
void ESP8266_Init(void)
{
	Usart2_Init(115200);//串口2初始化
	
	ESP8266_ExitUnvarnishSend();	//退出透传模式(透传模式下指令会直接发送到)
	if(ESP8266_AT_Test() == true)//测试
	{
		printf("ESP8266 Test OK!\n");
	}	
	else
		printf("ESP8266 Test FALSE!\n");
	
	ESP8266_TCP_Client();
}

void ESP8266_TCP_Client(void)
{
	ESP8266_STA();
	ESP8266_SendString(ENABLE, "test", Single_ID_0);
}

//配置Wifi为STA模式，TCP Client
void ESP8266_STA(void)
{
	printf("\r\n正在配置 ESP8266 .........\r\n");
	
	while(!ESP8266_Net_Mode_Choose(STA));//设置为STA模式
	
	ESP8266_Rst();//重启模块
	
	//while(!ESP8266_ServerStatus(DISABLE, 8080, 500));	//设置为服务器模式并设置端口号
	
	while(!ESP8266_Enable_MultipleId(DISABLE));	//单连接模式
	
	while(!ESP8266_JoinAP(Ssid, Pwd));	//连入WIFI
	
	//while(!ESP8266_IP("6.6.6.6"));//设置IP地址
	
	while(!ESP8266_Link_Server(TCP, TCP_Server_IP, TCP_Server_Port));	//连接服务器
	
	while(!ESP8266_Unvarnish_Send(1))	//设置为透传模式
	
	printf("\r\n配置 ESP8266 完毕\r\n");

}

//配置Wifi为STA + AP模式，路由模式
void ESP8266_STA_AP(void)
{
	if(ESP8266_Net_Mode_Choose(STA_AP) == true)//设置为STA+AP模式
	{
		if(ESP8266_BuildAP("Cloud_Home", "cloudhome", 11, 3) == true)//配置WiFi
		{
			ESP8266_Rst();//重启模块
			if(ESP8266_Enable_MultipleId(ENABLE) == true)//设置为多路连接模式
			{
				if(ESP8266_IP("6.6.6.6") == true)//设置IP地址
				{
					ESP8266_ServerStatus(ENABLE, 8080, 500);	//设置为服务器模式并设置端口号
				}
			}
		}	
	}
}

/******************************
 *	对ESP8266模块进行AT测试启动
 ******************************/
bool ESP8266_AT_Test(void)
{
	char count = 0;
	printf("开始测试 ESP8266 ........\r\n");
	delay_ms(500);
	while(count < 10)
	{
		//判断是否返回OK，否则循环10次
		if(ESP8266_Send_AT_Cmd("AT\r\n","OK", NULL, 500))
			return true;
		ESP8266_Rst();
		++count;
	}
	
	return false;
}
