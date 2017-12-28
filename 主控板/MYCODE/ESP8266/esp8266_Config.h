#ifndef __ESP8266_CONFIG_H_
#define __ESP8266_CONFIG_H_

#include "stm32f10x.h"
#include "stdbool.h"
#include "esp8266.h"

#define  Ssid				"jldsjafl"		//要连接的热点的名称
#define  Pwd				"poyu439670"		//要连接的热点的密钥

#define  TCP_Server_IP		"112.74.37.132"		//阿里云服务器的IP
//#define  TCP_Server_IP		"192.168.1.15"		//要连接的服务器的IP
#define  TCP_Server_Port	50001				//要连接的服务器的端口号

void ESP8266_Init(void);
void ESP8266_STA(void);
void ESP8266_STA_AP(void);
bool ESP8266_AT_Test(void);
void ESP8266_TCP_Client(void);

#endif
