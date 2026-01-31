#include "mpu6050.h"
#include "math.h"
#include "bluetooth.h"

float ax, ay, az, gx, gy, gz;												//6轴数据
float pitch = 0, roll = 0, yaw = 0;											//3轴角度

static float pitch_offset = 0, roll_offset = 0, yaw_offset = 0;			//偏移量 
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// 四元数
static float integralFBx = 0.0f, integralFBy = 0.0f, integralFBz = 0.0f;	// 积分误差项
static float gx_offset = 0, gy_offset = 0, gz_offset = 0;


static float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

static void Mahony_Update(float gx, float gy, float gz, float ax, float ay, float az)
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	
	float acc_norm = sqrtf(ax * ax + ay * ay + az * az);
	uint8 acc_valid = (acc_norm > 0.8f && acc_norm < 1.2f);
	
	if(acc_valid && !((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
	{
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		halfvx = q1 * q3 - q0 * q2;
		halfvy = q0 * q1 + q2 * q3;
		halfvz = q0 * q0 - 0.5f + q3 * q3;

		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		if(TWO_KI > 0.0f)
		{
			integralFBx += TWO_KI * halfex * (1.0f / SAMPLE_FREQ);
			integralFBy += TWO_KI * halfey * (1.0f / SAMPLE_FREQ);
			integralFBz += TWO_KI * halfez * (1.0f / SAMPLE_FREQ);
			gx += integralFBx;
			gy += integralFBy;
			gz += integralFBz;
		}

		gx += TWO_KP * halfex;
		gy += TWO_KP * halfey;
		gz += TWO_KP * halfez;
	}
	
	gx *= (0.5f * (1.0f / SAMPLE_FREQ));
	gy *= (0.5f * (1.0f / SAMPLE_FREQ));
	gz *= (0.5f * (1.0f / SAMPLE_FREQ));
	
	qa = q0;
	qb = q1;
	qc = q2;
	
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);
	
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

static void Quaternion_To_Euler(void)
{
	pitch = asinf(-2.0f * (q1 * q3 - q0 * q2)) * 57.29578f;
	roll = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2)) * 57.29578f;
	yaw = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3)) * 57.29578f;
}

static void Gyro_Calibrate(void)
{
    float gx_sum = 0, gy_sum = 0, gz_sum = 0;
    
    for(uint16 i = 0; i < 200; i++)
    {
        mpu6050_get_gyro();
        gx_sum += mpu6050_gyro_transition(mpu6050_gyro_x);
        gy_sum += mpu6050_gyro_transition(mpu6050_gyro_y);
        gz_sum += mpu6050_gyro_transition(mpu6050_gyro_z);
        system_delay_ms(2);
    }
    
    gx_offset = gx_sum / 200.0f;
    gy_offset = gy_sum / 200.0f;
    gz_offset = gz_sum / 200.0f;
}

/*MPU校准*/
void Mpu6050_Calibrate(void)
{
	float p_sum = 0;
	uint8 cnt = 0;
	
	for(uint16 i = 0; i < 1000; i++)
	{
		Mpu6050_Read();
		system_delay_ms(1);
	}
	
	while(cnt < 25)
	{
		Mpu6050_Read();
		p_sum += pitch;
		cnt++;
		system_delay_ms(2);
	}
	
	pitch_offset = p_sum / 25.0f;
}

/*MPU初始化*/
void Mpu6050_Init(void)
{
	mpu6050_init();
	
	q0 = 1.0f; q1 = 0.0f; q2 = 0.0f; q3 = 0.0f;
	
	ips200_show_string(0, 240, "Gyro Calib...  ");
	Gyro_Calibrate();
	ips200_show_string(0, 240, "Calibrating... ");
	Mpu6050_Calibrate();
	ips200_show_string(0, 240, "Ready!         ");
}

/*MPU读取*/
void Mpu6050_Read(void)
{
	mpu6050_get_acc();
	mpu6050_get_gyro();
	ax = mpu6050_acc_transition(mpu6050_acc_x);
	ay = mpu6050_acc_transition(mpu6050_acc_y);
	az = mpu6050_acc_transition(mpu6050_acc_z);
	
	gx = mpu6050_gyro_transition(mpu6050_gyro_x) - gx_offset;
	gy = -(mpu6050_gyro_transition(mpu6050_gyro_y) - gy_offset);
	gz = mpu6050_gyro_transition(mpu6050_gyro_z) - gz_offset;
	
    float gx_rad = gx * 0.0174533f;
    float gy_rad = gy * 0.0174533f;
    float gz_rad = gz * 0.0174533f;

    Mahony_Update(gy_rad, gx_rad, -gz_rad, -ax, -ay, az);
    Quaternion_To_Euler();
	
	pitch -= pitch_offset;
	pitch = -pitch;
}

/*显示*/
void Mpu6050_Show(void)
{
	ips200_show_string(0,256,"Pitch:");
//	ips200_show_string(0,272,"Roll:");ips200_show_string(0,288,"Yaw:");
	ips200_show_float(48,256,pitch,3,1);
//	ips200_show_float(40,272,roll,3,1);ips200_show_float(32,288,yaw,3,1);
	BlueSerial_Printf("[plot,%f]",pitch);
}
