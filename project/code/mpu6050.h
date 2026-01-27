#ifndef __MPU6050_H
#define __MPU6050_H

#include "zf_common_headfile.h"

#define MPU_ADDR    0x68

extern uint8 mpuflag;
extern float ax,ay,az,gx,gy,gz;
extern float pitch, roll, yaw;

uint8 MPU_Write_Len(uint8 addr, uint8 reg, uint8 len, uint8 *buf);
uint8 MPU_Read_Len(uint8 addr, uint8 reg, uint8 len, uint8 *buf);

void Mpu6050_Init(void);
void Mpu6050_Read(void);
void Mpu6050_Show(void);

#endif
