#ifndef __MPU6050_H
#define __MPU6050_H

#include "zf_common_headfile.h"

// 卡尔曼滤波结构体
typedef struct {
    float Q_angle;   // 角度过程噪声协方差
    float Q_bias;    // 偏差过程噪声协方差
    float R_measure; // 测量噪声协方差
    
    float angle;     // 计算出的角度（滤波后）
    float bias;      // 陀螺仪偏差
    float rate;      // 未经滤波的角速度
    
    float P[2][2];   // 误差协方差矩阵
} KalmanFilter;

extern float gyro_pitch, acc_pitch, pitch;
extern float yaw;
extern KalmanFilter KF;

void Kalman_Init(KalmanFilter* kf,float Q_angle,float Q_bias,float R_measure);
void Calculate_Attitude(void);

#endif
