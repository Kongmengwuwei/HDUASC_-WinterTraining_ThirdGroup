#ifndef __MENU_H
#define __MENU_H

#include "zf_common_headfile.h"

/*ips200_show_int(224,304,1,1);фад╩╠ъ╫Г*/
#define IPS200_TYPE		(IPS200_TYPE_SPI)

typedef enum {
	Main,
	CarCheck,
	PID,
	Edit
} Menu;

typedef enum {
	Carmode,
	Angle,
	Speed,
	Turn,
	Position
} Param;

#define ROW_LENGTH		(16)

extern float parameter[5][3];

void Menu_Init(void);
void Menu_Update(void);

#endif
