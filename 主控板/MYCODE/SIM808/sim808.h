#ifndef __SIM808_H_
#define __SIM808_H_

#include "stm32f10x.h"

#define SIM_OK 				0
#define SIM_COMMUNTION_ERR 	0xff
#define SIM_CPIN_ERR 		0xfe
#define SIM_CREG_FAIL 		0xfd

u8 SIM_Init(void);
u8 sim900a_work_test(void);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* sim900a_check_cmd(u8 *str);

#endif
