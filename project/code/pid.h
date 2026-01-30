#ifndef __PID_H
#define __PID_H

#include "zf_common_headfile.h"

typedef struct {
	float Target;
	float Actual;
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
} PID_t;

extern uint8 RunFlag;
extern int16 LeftPWM, RightPWM;
extern int16 AvePWM, DifPWM;
extern float LeftSpeed, RightSpeed;
extern float AveSpeed, DifSpeed;

extern PID_t AnglePID, SpeedPID;

void PID_Init(PID_t *p);
void PID_Update(PID_t *p);
void Angle_Tweak (void);
void Speed_Tweak (void);

#endif
