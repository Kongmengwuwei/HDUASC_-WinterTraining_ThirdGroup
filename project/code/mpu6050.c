#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

soft_iic_info_struct mpu_iic;			//软件IIC（用于DMP）
uint8 mpuflag = 0;						//MPU正常初始化标识
float ax,ay,az,gx,gy,gz;				//6轴数据
float pitch = 0, roll = 0, yaw = 0;		//3轴角度

/*MPU/DMP初始化*/
void Mpu6050_Init(void)
{
	system_delay_ms(50);															//延时稳定
	
	soft_iic_init(&mpu_iic, MPU_ADDR, 10, MPU6050_SCL_PIN, MPU6050_SDA_PIN);		//开启软件IIC
	
	system_delay_ms(50);
	
	uint8 dmpflag;																	//DMP正常标识
	
	uint8 retry = 3;																//DMP错误就重试
	do {
		dmpflag = mpu_dmp_init();
		if(dmpflag == 0) break;
		system_delay_ms(100);
		retry--;
	} while(retry > 0);
	
	ips200_show_string(0, 304, "DMP:");												//0为正常
	ips200_show_int(32, 304, dmpflag, 2);
	
	mpu6050_init();																	//开启MPURead
}

/*MPU读取*/
void Mpu6050_Read(void)
{
		mpu_dmp_get_data(&pitch, &roll, &yaw);
		
		mpu6050_get_acc();
		mpu6050_get_gyro();
		ax = mpu6050_acc_transition(mpu6050_acc_x);ay = mpu6050_acc_transition(mpu6050_acc_y);az = mpu6050_acc_transition(mpu6050_acc_z);
		gx = mpu6050_gyro_transition(mpu6050_gyro_x);gy = mpu6050_gyro_transition(mpu6050_gyro_y);gz = mpu6050_gyro_transition(mpu6050_gyro_z);
}

/*MPU数据屏幕显示*/
void Mpu6050_Show(void)
{
	ips200_show_string(0,160,"Ax:");ips200_show_string(0,176,"Ay:");ips200_show_string(0,192,"Az:");
	ips200_show_string(0,208,"Gx:");ips200_show_string(0,224,"Gy:");ips200_show_string(0,240,"Gz:");
	ips200_show_string(0,256,"Pitch:");ips200_show_string(0,272,"Roll:");ips200_show_string(0,288,"Yaw:");
	
	ips200_show_float(24,160,ax,3,1);ips200_show_float(24,176,ay,3,1);ips200_show_float(24,192,az,3,1);
	ips200_show_float(24,208,gx,3,1);ips200_show_float(24,224,gy,3,1);ips200_show_float(24,240,gz,3,1);
	ips200_show_float(48,256,pitch,3,1);ips200_show_float(40,272,roll,3,1);ips200_show_float(32,288,yaw,3,1);
}

/*DMP前置函数*/
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


