#ifndef __SENSOR_H
#define __SENSOR_H

#include "zf_common_headfile.h"

#define SenSor1		(B31)
#define SenSor2		(B30)
#define SenSor3		(B29)
#define SenSor4		(B28)
#define Flag_Hold	(5)

void Sensor_Init(void);
uint8 Sensor_Check (void);

extern uint8 flag, prev_stat, curr_stat;
extern uint8 stat1, stat2, stat3, stat4 ;

#endif
