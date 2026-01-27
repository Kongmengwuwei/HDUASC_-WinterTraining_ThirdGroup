#ifndef __MOTOR_H
#define __MOTOR_H

#include "zf_common_headfile.h"

#define MAX_DUTY                 (50 )                // 最大 MAX_DUTY% 占空比
#define MOTOR1_1                 (C9 )
#define MOTOR1_2                 (D0)
#define MOTOR1_PWM               (PWM2_MODULE1_CHA_C8)

#define MOTOR2_1                 (C7 )
#define MOTOR2_2                 (D1)
#define MOTOR2_PWM               (PWM2_MODULE0_CHA_C6)

#define ENCODER_1                   (QTIMER1_ENCODER1)
#define ENCODER_1_A                 (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_1_B                 (QTIMER1_ENCODER1_CH2_C1)

#define ENCODER_2                   (QTIMER1_ENCODER2)
#define ENCODER_2_A                 (QTIMER1_ENCODER2_CH1_C2)
#define ENCODER_2_B                 (QTIMER1_ENCODER2_CH2_C24)

void Motor_Init();
void Set_Moter1(int16 pwm1);
void Set_Moter2(int16 pwm2);

#endif
