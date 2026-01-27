#ifndef __MENU_H
#define __MENU_H

#include "zf_common_headfile.h"

/*ips200_show_int(224,304,1,1);屏幕边界*/
#define IPS200_TYPE		(IPS200_TYPE_SPI)

#define MODE_COUNT		(5)						//5个发车模式
#define PID_COUNT		(3)						//PID
#define ROW_MODE		(16)						//参数行
#define ROW_KP			(48)
#define ROW_KI			(64)
#define ROW_KD			(80)

extern float pidnum[MODE_COUNT][PID_COUNT];
extern uint8_t carmod;

void Menu_Init(void);
void Menu_Update(void);

#endif
