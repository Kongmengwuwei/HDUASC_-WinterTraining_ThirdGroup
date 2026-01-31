#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"

soft_iic_info_struct mpu_iic;										//软件IIC（用于DMP）
uint8 mpuflag = 0;													//mpu标识
float ax, ay, az, gx, gy, gz;										//6轴数据
float pitch = 0, roll = 0, yaw = 0;									//3轴角度

static float pitch_offset = 0, roll_offset = 0, yaw_offset = 0;	//偏移量
static uint8 dmp_stable = 0;										//dmp稳定标识

/*等待DMP稳定*/
static void Wait_DMP_Stable(void)
{
	float pitch_raw, roll_raw, yaw_raw;
	float start_pitch = 0, current_pitch = 0;
	uint16 wait_sec = 0;
	
	ips200_show_string(0, 128, "Waiting...     ");
	
	while(mpu_dmp_get_data(&pitch_raw, &roll_raw, &yaw_raw) != 0)
		system_delay_ms(10);
	start_pitch = pitch_raw;
	
	while(wait_sec < 30)
	{
		for(uint16 i = 0; i < 100; i++)
		{
			if(mpu_dmp_get_data(&pitch_raw, &roll_raw, &yaw_raw) == 0)
				current_pitch = pitch_raw;
			system_delay_ms(10);
		}
		wait_sec++;

		ips200_show_string(0, 144, "T:");
		ips200_show_int(16, 144, wait_sec, 2);
		ips200_show_string(40, 144, "D:");
		ips200_show_float(56, 144, fabsf(current_pitch - start_pitch), 2, 2);
        
		if(fabsf(current_pitch - start_pitch) < 0.1f)
			break;
		start_pitch = current_pitch;
	}
	
	ips200_show_string(0, 128, "Stable!        ");
}

/*校准*/
void Mpu6050_Calibrate(void)
{
	float pitch_raw, roll_raw, yaw_raw;
	float p_sum = 0, r_sum = 0, y_sum = 0;
	uint8 cnt = 0;
	
	while(cnt < 10)													// 取最后10次平均
	{
		if(mpu_dmp_get_data(&pitch_raw, &roll_raw, &yaw_raw) == 0)
		{
			p_sum += pitch_raw;
//			r_sum += roll_raw;
//			y_sum += yaw_raw;
			cnt++;
		}
		system_delay_ms(10);
	}
	
	pitch_offset = p_sum / 10.0f;
//	roll_offset = r_sum / 10.0f;
//	yaw_offset = y_sum / 10.0f;
}

/*MPU/DMP初始化*/
void Mpu6050_Init(void)
{
	system_delay_ms(50);
	soft_iic_init(&mpu_iic, MPU_ADDR, 10, MPU6050_SCL_PIN, MPU6050_SDA_PIN);
	system_delay_ms(50);
	
	uint8 dmpflag;
	uint8 retry = 3;
	do {
		dmpflag = mpu_dmp_init();
		if(dmpflag == 0) break;
		system_delay_ms(100);
		retry--;
	} while(retry > 0);
	
	ips200_show_string(120, 128, "DMP:");
	ips200_show_int(152, 128, dmpflag, 1);
	
	mpu6050_init();
	
//	Wait_DMP_Stable();
//	
//	ips200_show_string(0, 128, "Calibrating... ");
//	Mpu6050_Calibrate();
	
	dmp_stable = 1;
	ips200_show_string(0, 128, "Ready!         ");
}

/*MPU读取*/
void Mpu6050_Read(void)
{
	float pitch_raw, roll_raw, yaw_raw;
	
	if(mpu_dmp_get_data(&pitch_raw, &roll_raw, &yaw_raw) == 0)
	{
		if(dmp_stable)
		{
			pitch = pitch_raw - pitch_offset;
//			roll = roll_raw - roll_offset;
//			yaw = yaw_raw - yaw_offset;
		}
		else
		{
			pitch = pitch_raw;
//			roll = roll_raw;
//			yaw = yaw_raw;
		}
	}
	
	mpu6050_get_acc();
	mpu6050_get_gyro();
	ax = mpu6050_acc_transition(mpu6050_acc_x);
	ay = mpu6050_acc_transition(mpu6050_acc_y);
	az = mpu6050_acc_transition(mpu6050_acc_z);
	gx = mpu6050_gyro_transition(mpu6050_gyro_x);
	gy = mpu6050_gyro_transition(mpu6050_gyro_y);
	gz = mpu6050_gyro_transition(mpu6050_gyro_z);
}

/*显示*/
void Mpu6050_Show(void)
{
	ips200_show_string(0,160,"Ax:");ips200_show_string(0,176,"Ay:");ips200_show_string(0,192,"Az:");
	ips200_show_string(0,208,"Gx:");ips200_show_string(0,224,"Gy:");ips200_show_string(0,240,"Gz:");
	ips200_show_string(0,256,"Pitch:");
//	ips200_show_string(0,272,"Roll:");ips200_show_string(0,288,"Yaw:");
	
	ips200_show_float(24,160,ax,3,1);ips200_show_float(24,176,ay,3,1);ips200_show_float(24,192,az,3,1);
	ips200_show_float(24,208,gx,3,1);ips200_show_float(24,224,gy,3,1);ips200_show_float(24,240,gz,3,1);
	ips200_show_float(48,256,pitch,3,1);
//	ips200_show_float(40,272,roll,3,1);ips200_show_float(32,288,yaw,3,1);
}

/*DMP前置*/
uint8 MPU_Write_Len(uint8 addr, uint8 reg, uint8 len, uint8 *buf)
{
	soft_iic_write_8bit_registers(&mpu_iic, reg, buf, len);
	return 0;
}

uint8 MPU_Read_Len(uint8 addr, uint8 reg, uint8 len, uint8 *buf)
{
	soft_iic_read_8bit_registers(&mpu_iic, reg, buf, len);
	return 0;
}