#ifndef __MENU_H
#define __MENU_H

#include "zf_common_headfile.h"

/*ips200_show_int(224,304,1,1);фад╩╠ъ╫Г*/
#define IPS200_TYPE		(IPS200_TYPE_SPI)

typedef enum {
	Carmode,
	PID,
	Angle,
	Speed,
	Turn,
	Position
} Menu;


#define ROW_LENGTH		(16)

extern float pidnum[5][4][3];
extern uint8_t carmode;

void Menu_Init(void);
void Menu_Update(void);

#endif
