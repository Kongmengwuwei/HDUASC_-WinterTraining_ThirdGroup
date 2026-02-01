#ifndef __SENSOR_H
#define __SENSOR_H

#include "zf_common_headfile.h"

#define SenSor1		(C16)
#define SenSor2		(C17)
#define SenSor3		(C20)
#define SenSor4		(C21)
#define Flag_Hold	(5)

void Sensor_Init(void);
uint8 Sensor_Check (void);

extern uint8 flag, prev_stat, curr_stat;

#endif
