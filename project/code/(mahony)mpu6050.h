#ifndef __MPU6050_H
#define __MPU6050_H

#include "zf_common_headfile.h"

#define MPU_ADDR			0x68
#define SAMPLE_FREQ			500.0f							// 采样频率(Hz)
#define TWO_KP				15.0f							//比例增益
#define TWO_KI				0.3f							//积分增益

extern float ax,ay,az,gx,gy,gz;
extern float pitch, roll, yaw;

void Mpu6050_Init(void);
void Mpu6050_Read(void);
void Mpu6050_Show(void);

#endif
