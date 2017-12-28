#ifndef __ESP8266_CONFIG_H_
#define __ESP8266_CONFIG_H_

#include "stm32f10x.h"
#include "stdbool.h"
#include "esp8266.h"

#define  Ssid				"jldsjafl"		//Ҫ���ӵ��ȵ������
#define  Pwd				"poyu439670"		//Ҫ���ӵ��ȵ����Կ

#define  TCP_Server_IP		"112.74.37.132"		//�����Ʒ�������IP
//#define  TCP_Server_IP		"192.168.1.15"		//Ҫ���ӵķ�������IP
#define  TCP_Server_Port	50001				//Ҫ���ӵķ������Ķ˿ں�

void ESP8266_Init(void);
void ESP8266_STA(void);
void ESP8266_STA_AP(void);
bool ESP8266_AT_Test(void);
void ESP8266_TCP_Client(void);

#endif
