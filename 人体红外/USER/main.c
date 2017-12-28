#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "systick.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
	return ch;
}

int main(void)
{
	Led_Init();
	Systick_Init();
	Usart1_Init();
	
	
	USART_SendData(USART1, 'h');
	//printf("%s",buff);
	
	while(1)
	{
		if(PAin(3) == 1)
		{
			PCout(13)=0;
    }
		else if(PAin(3) == 0)
    {
			PCout(13)=1; 
    }
  
	}
}
