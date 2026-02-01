#ifndef __PID_H
#define __PID_H

#include "zf_common_headfile.h"

//PID参数结构体
typedef struct {
	float Target;
	float Actual;
	float Actual1;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	
	float ErrorIntMax;
	float ErrorIntMin;
	
	float OutMax;
	float OutMin;
	
	float OutOffset;
} PID_t;

extern uint8 RunFlag;
extern int16 LeftPWM, RightPWM;
extern int16 AvePWM, DifPWM;
extern float LeftSpeed, RightSpeed, Last_LeftSpeed, Last_RightSpeed;
extern float AveSpeed, DifSpeed;

extern PID_t AnglePID, SpeedPID, TurnPID;

void PID_Init(PID_t *p);
void PID_Update(PID_t *p);
void Angle_Tweak (void);
void Speed_Tweak (void);
void Turn_Tweak(void);

#endif
