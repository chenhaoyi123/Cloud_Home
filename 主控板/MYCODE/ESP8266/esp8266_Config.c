#include "esp8266_Config.h"

//ESP8266��ʼ��
void ESP8266_Init(void)
{
	Usart2_Init(115200);//����2��ʼ��
	
	ESP8266_ExitUnvarnishSend();	//�˳�͸��ģʽ(͸��ģʽ��ָ���ֱ�ӷ��͵�)
	if(ESP8266_AT_Test() == true)//����
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

//����WifiΪSTAģʽ��TCP Client
void ESP8266_STA(void)
{
	printf("\r\n�������� ESP8266 .........\r\n");
	
	while(!ESP8266_Net_Mode_Choose(STA));//����ΪSTAģʽ
	
	ESP8266_Rst();//����ģ��
	
	//while(!ESP8266_ServerStatus(DISABLE, 8080, 500));	//����Ϊ������ģʽ�����ö˿ں�
	
	while(!ESP8266_Enable_MultipleId(DISABLE));	//������ģʽ
	
	while(!ESP8266_JoinAP(Ssid, Pwd));	//����WIFI
	
	//while(!ESP8266_IP("6.6.6.6"));//����IP��ַ
	
	while(!ESP8266_Link_Server(TCP, TCP_Server_IP, TCP_Server_Port));	//���ӷ�����
	
	while(!ESP8266_Unvarnish_Send(1))	//����Ϊ͸��ģʽ
	
	printf("\r\n���� ESP8266 ���\r\n");

}

//����WifiΪSTA + APģʽ��·��ģʽ
void ESP8266_STA_AP(void)
{
	if(ESP8266_Net_Mode_Choose(STA_AP) == true)//����ΪSTA+APģʽ
	{
		if(ESP8266_BuildAP("Cloud_Home", "cloudhome", 11, 3) == true)//����WiFi
		{
			ESP8266_Rst();//����ģ��
			if(ESP8266_Enable_MultipleId(ENABLE) == true)//����Ϊ��·����ģʽ
			{
				if(ESP8266_IP("6.6.6.6") == true)//����IP��ַ
				{
					ESP8266_ServerStatus(ENABLE, 8080, 500);	//����Ϊ������ģʽ�����ö˿ں�
				}
			}
		}	
	}
}

/******************************
 *	��ESP8266ģ�����AT��������
 ******************************/
bool ESP8266_AT_Test(void)
{
	char count = 0;
	printf("��ʼ���� ESP8266 ........\r\n");
	delay_ms(500);
	while(count < 10)
	{
		//�ж��Ƿ񷵻�OK������ѭ��10��
		if(ESP8266_Send_AT_Cmd("AT\r\n","OK", NULL, 500))
			return true;
		ESP8266_Rst();
		++count;
	}
	
	return false;
}
