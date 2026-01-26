#ifndef __MPU6050_H
#define __MPU6050_H

#include "zf_common_headfile.h"

extern uint8 mpuflag;
extern float ax,ay,az,gx,gy,gz;

void Mpu6050_Init(void);
void Mpu6050_Read(void);
void Mpu6050_Show(void);

#endif
