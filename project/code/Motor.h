#ifndef __MOTOR_H
#define __MOTOR_H

#include "zf_common_headfile.h"

#define MOTOR1_1                 (C10)
#define MOTOR1_2                 (D1)
#define MOTOR1_PWM   						 (PWM2_MODULE2_CHB_C11)

#define MOTOR2_1               	 (D2)
#define MOTOR2_2              	 (D0)
#define MOTOR2_PWM            	 (PWM2_MODULE3_CHB_D3)

void Motor_Init();
void Set_Motor1(int16 pwm1);
void Set_Motor2(int16 pwm2);

#endif
