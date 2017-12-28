#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"
#include "sys.h"

#define	 LED_B_5	PBout(5)
#define  LED_E_5 	PEout(5)
#define  LED_E_6	PEout(6)

void Led_Init(void);

#endif
