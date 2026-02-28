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

extern int32 LeftPWM, RightPWM;
extern int32 AvePWM, DifPWM;
extern PID_t GyroPID, AnglePID, SpeedPID, TurnPID, TracePID;

void PID_Init(PID_t *p);
void PID_Update(PID_t *p);
void Gyro_Tweak (void);
void Angle_Tweak (void);
void Speed_Tweak (void);
void Turn_Tweak(void);
void Trace_Tweak(void);

#endif
