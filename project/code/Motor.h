#ifndef __MOTOR_H
#define __MOTOR_H

#include "zf_common_headfile.h"

#define MOTOR1_1                 (D2)
#define MOTOR1_2                 (D0)
#define MOTOR1_PWM               (PWM2_MODULE3_CHB_D3)

#define MOTOR2_1                 (C10 )
#define MOTOR2_2                 (D1)
#define MOTOR2_PWM               (PWM2_MODULE2_CHB_C11)

#define ENCODER_1                   (QTIMER2_ENCODER2)
#define ENCODER_1_A                 (QTIMER2_ENCODER2_CH1_C5)
#define ENCODER_1_B                 (QTIMER2_ENCODER2_CH2_C25)

#define ENCODER_2                   (QTIMER2_ENCODER1)
#define ENCODER_2_A                 (QTIMER2_ENCODER1_CH1_C3)
#define ENCODER_2_B                 (QTIMER2_ENCODER1_CH2_C4)

void Motor_Init();
void Set_Motor1(int16 pwm1);
void Set_Motor2(int16 pwm2);

#endif
