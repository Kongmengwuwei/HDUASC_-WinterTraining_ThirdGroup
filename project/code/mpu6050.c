#include "mpu6050.h"
#include "math.h"
#include "stdio.h"

float time=0.001;  //时间
float Offset=0;   //偏移

float gyro_yaw , gyro_pitch , acc_yaw , acc_pitch;
float yaw, pitch, roll;
int16 AX, AY, AZ;
KalmanFilter KF;


/*初始化卡尔曼滤波*/
//三个参数：角度过程噪声，零偏过程噪声，测量噪声
void Kalman_Init(KalmanFilter* kf,float Q_angle,float Q_bias,float R_measure) {
    kf->Q_angle = Q_angle;
    kf->Q_bias = Q_bias;
    kf->R_measure = R_measure;

    kf->angle = 0.0f;
    kf->bias = -1.5f;
    
    kf->P[0][0] = 0.0f;
    kf->P[0][1] = 0.0f;
    kf->P[1][0] = 0.0f;
    kf->P[1][1] = 0.0f;
}

/* 卡尔曼滤波计算 */
float Kalman_Calculate(KalmanFilter* kf, float newAngle, float newRate, float time, float* corrected_rate) {

    kf->rate = newRate - kf->bias;
    kf->angle += time * kf->rate;
    
    kf->P[0][0] += time * (time * kf->P[1][1] - kf->P[0][1] - kf->P[1][0] + kf->Q_angle);
    kf->P[0][1] -= time * kf->P[1][1];
    kf->P[1][0] -= time * kf->P[1][1];
    kf->P[1][1] += kf->Q_bias * time;
    
    float S = kf->P[0][0] + kf->R_measure;
    float K[2];
    K[0] = kf->P[0][0] / S;
    K[1] = kf->P[1][0] / S;
    
    float y = newAngle - kf->angle;
    kf->angle += K[0] * y;
    kf->bias += K[1] * y;
    
    float P00_temp = kf->P[0][0];
    float P01_temp = kf->P[0][1];
    
    kf->P[0][0] -= K[0] * P00_temp;
    kf->P[0][1] -= K[0] * P01_temp;
    kf->P[1][0] -= K[1] * P00_temp;
    kf->P[1][1] -= K[1] * P01_temp;
		
		
		if(corrected_rate != NULL) {
      *corrected_rate = kf->rate;
    }
    
    return kf->angle;
}

void Calculate_Attitude(void)  //姿态解算
{
	
		mpu6050_get_gyro();
		mpu6050_get_acc();
		
		//yaw
		gyro_yaw += (mpu6050_gyro_transition(mpu6050_gyro_z) * time);
		yaw = gyro_yaw;	
		//pitch
		AX = mpu6050_acc_x;
		AY = mpu6050_acc_y;
		AZ = mpu6050_acc_z;
		// 计算加速度计角度
		float acc_pitch = -atan2(AX, sqrt(AY*AY + AZ*AZ)) * 57.29578f; // 180/π ≈ 57.29578
		// 获取陀螺仪原始角速度（转换为°/s）
		float gyro_rate_raw = (mpu6050_gyro_y / 32768.0f) * 2000.0f;  // 假设量程为±2000°/s
		
		pitch=Kalman_Calculate(&KF, acc_pitch, gyro_rate_raw, time, &gyro_pitch);
}
