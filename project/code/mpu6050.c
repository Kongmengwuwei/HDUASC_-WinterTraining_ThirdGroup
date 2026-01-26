#include "mpu6050.h"

uint8 mpuflag = 0;
float ax,ay,az,gx,gy,gz;

void Mpu6050_Init(void)
{
	if(!mpu6050_init())mpuflag = 1;
}

void Mpu6050_Read(void)
{
	if(mpuflag)
	{
		mpu6050_get_acc();
		mpu6050_get_gyro();
		ax = mpu6050_acc_transition(mpu6050_acc_x);ay = mpu6050_acc_transition(mpu6050_acc_y);az = mpu6050_acc_transition(mpu6050_acc_z);
		gx = mpu6050_gyro_transition(mpu6050_gyro_x);gy = mpu6050_gyro_transition(mpu6050_gyro_y);gz = mpu6050_gyro_transition(mpu6050_gyro_z);
	}
}

void Mpu6050_Show(void)
{
	ips200_show_float(0,48,ax,3,1);ips200_show_float(56,48,ay,3,1);ips200_show_float(112,48,az,3,1);
	ips200_show_float(0,64,gx,3,1);ips200_show_float(56,64,gy,3,1);ips200_show_float(112,64,gz,3,1);
}