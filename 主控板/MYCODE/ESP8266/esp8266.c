#include "esp8266.h"

/************************************************
 * ����ESP8266ģ��
 * ��ƽ�ȵͺ�ߣ���ESP8266����GPIO16���Ÿ�λ����
 * ��ʹ�����AT+RST\r\n
*************************************************/
bool ESP8266_Rst(void)
{
	char str[20];

	sprintf(str, "AT+RST\r\n");
	
	return  ESP8266_Send_AT_Cmd(str, "ready", NULL, 5000);
}

/******************************************
 *	��ESP8266ģ�鷢��ATָ��
 *	cmd�������͵�ָ��
 *	response���ڴ�����Ӧ(���أ��ɹ�)��ΪNULL��ʾ����Ҫ��ʾ
 *	error������ķ���ֵ
 *	time���ȴ���Ӧ��ʱ��
 *	����ֵ��0������ʧ�ܣ�1�����ͳɹ�
 ******************************************/
bool ESP8266_Send_AT_Cmd(char *cmd, char *response, char *error, u32 waittime)
{
	printf("%s\n", cmd);
	Usart2_Send_String(cmd, strlen(cmd));//AT����Ӵ���2��8266����

	if(response == NULL && error == NULL)	 //����Ҫ��������
		return true;
	
	delay_ms(waittime);	  //��ʱtimeʱ��
	printf("ESP8266_Send_AT_Cmd[%d]��%s\n\n", __LINE__, rx_buffer);//�򴮿�1�����ԣ���������

	if(response != NULL && error != NULL)
		return ((bool)strstr((char *)rx_buffer, response ) || (bool)strstr((char *)rx_buffer, error));
		
	else if(response != 0)
		return ((bool)strstr((char *)rx_buffer, response));
		
	else
		return ((bool)strstr((char *)rx_buffer, error));
}

/****************************************************************
 *	ѡ��ESP8266ģ��Ĺ���ģʽ
 *	Mode������ģʽ(1.Stationģʽ��2.APģʽ��3.AP��Stationģʽ)
*	���أ�1.ѡ��ɹ� 
		  0.ѡ��ʧ��
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
 *	ESP8266ģ�������ⲿWiFi
 *	���룺SSID��WiFi�����ַ���
		  pPassWord��WiFi�����ַ���
 *	����1�����ӳɹ� 0������ʧ��
*************************************/
bool ESP8266_JoinAP(char *SSID, char *PassWord)
{
	char Cmd[120];

	sprintf(Cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PassWord);
	
	return ESP8266_Send_AT_Cmd(Cmd, "OK", NULL, 5000);
}

/********************************************************************
 *	ESP8266����WiFi�ȵ�
 *	���룺name WiFi�����ַ���
		  pwd WiFi�����ַ���
		  ͨ����:11��
		  ���ܷ�ʽ��(0.OPEN��1.WEP��2.WPA_PSK��3.WPA2_PSK��4.WPA_WPA2_PSK)
*********************************************************************/
bool ESP8266_BuildAP(char *name, char *pwd, u8 num, u8 type)
{
	char Cmd[50];
	sprintf(Cmd, "AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n", name, pwd, num, type);
	
	return ESP8266_Send_AT_Cmd(Cmd, "OK", NULL, 500);
}

/*************************************
 *	ESP8266ģ����������ģʽ(0.��·����ģʽ,1.��·����ģʽ)
 *	���룺ENABLE->1; DISABLE->0
 *	���أ�1.���óɹ� 
		  0.����ʧ��
**************************************/
bool ESP8266_Enable_MultipleId(FunctionalState NewState)
{
	char Str[20];
	sprintf(Str, "AT+CIPMUX=%d\r\n", (NewState ? 1 : 0));
	
	return ESP8266_Send_AT_Cmd(Str, "OK", NULL, 500);
}

//����IP��ַ
bool ESP8266_IP(char *ip)
{
	char str[20];
	sprintf(str, "AT+CIPAP=\"%s\"\r\n", ip);
	
	return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
}

/********************
 *	����Ϊ͸��ģʽ
 */
bool ESP8266_Unvarnish_Send(u8 MODE)
{
	char str[20];
	switch(MODE)
	{
		//��͸��ģʽ
		case 0:
			sprintf(str, "AT+CIPMODE=%d\r\n", MODE);
			return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
		//͸��ģʽ
		case 1:
			sprintf(str, "AT+CIPMODE=%d\r\n", MODE);
			return ESP8266_Send_AT_Cmd(str, "OK", NULL, 500);
		default:
			return false;
	}
}

/********************************************
 *	ESP8266ģ�������ⲿ������(����TCP/UDP����)
 *	protocol��	����Э��
 *	ip��		������IP�ַ���
 *	ComNum��	�������˿��ַ���
 *	���أ�		1.���ӳɹ� 
				0.����ʧ��
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
 * ����  ��ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��ENABLE->1; DISABLE->0
 *       ��PortNum���������˿ں��ַ���
 * ����  ��1.�����ɹ�
 *         0.����ʧ��
 * ����  �����ⲿ����
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
 * ����  ����ȡESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
 * ����  ����
 * ����  : 2�����ip
 *         3����������
 *         3��ʧȥ����
 *         0����ȡ״̬ʧ��
 * ����  �����ⲿ����
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
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��
		   ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
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
 *	ESP8266ģ�鷢���ַ���
 *	���룺ENABLE->1; DISABLE->0
 *	Mode�������Ƿ���ʹ����͸��ģʽ
 *	Str��Ҫ���͵��ַ���
 *	Id�����ĸ�ID�����ַ���
 *	����1�����ͳɹ� 0������ʧ��
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
 *	����ESP8266ģ�����͸����������
 *	����1�����óɹ� 0������ʧ��
************************************/
bool ESP8266_UnvarnishSend(void)
{
	if (!ESP8266_Send_AT_Cmd("AT+CIPMODE=1\r\n", "OK", NULL, 500))
		return false;
	
	return 
	  	ESP8266_Send_AT_Cmd("AT+CIPSEND\r\n", "OK", ">", 500 );
}

/********************************************
 * ����  ������WF-ESP8266ģ���˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
**********************************************/
void ESP8266_ExitUnvarnishSend(void)
{
	delay_ms(1000);
	
	Usart2_Send_String("+++", strlen("+++"));
	
	delay_ms(500); 
	
}

/******************
 *	���������ַ��Ƚ�
********************/
bool ESP8266_Compare(char *recdata)
{
 	strstr((char *)rx_buffer, recdata);
	
	return 0;
}

