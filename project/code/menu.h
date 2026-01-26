#ifndef __MENU_H
#define __MENU_H

#include "zf_common_headfile.h"
#include "flash.h"

#define min 			0
#define max 			32
#define IPS200_TYPE		(IPS200_TYPE_SPI)
extern float num[];

void Menu_Init(void);
void Menu_Update(void);

#endif
