#include "pid.h"
#include "mpu6050.h"
#include "Motor.h"
#include "menu.h"

uint8 RunFlag = 1;
int16 LeftPWM = 0, RightPWM = 0;
int16 AvePWM = 0, DifPWM = 0;
float LeftSpeed = 0, RightSpeed = 0;
float AveSpeed = 0, DifSpeed = 0;

PID_t AnglePID = {
	.OutMax = 100,
	.OutMin = -100,
};

PID_t SpeedPID = {
	.OutMax = 20,
	.OutMin = -20,
	
	.ErrorIntMax = 150,
	.ErrorIntMin = -150,
};
PID_t TurnPID = {
	.OutMax = 0,
	.OutMin = -50,
};
void PID_Init(PID_t *p)
{
	p->Target = 0;
	p->Actual = 0;
	p->Out = 0;
	p->Error0 = 0;
	p->Error1 = 0;
	p->ErrorInt = 0;
}

void PID_Update(PID_t *p)
{
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
	
	/*外环误差积分（累加）*/
	/*如果Ki不为0，才进行误差积分，这样做的目的是便于调试*/
	/*因为在调试时，我们可能先把Ki设置为0，这时积分项无作用，误差消除不了，误差积分会积累到很大的值*/
	/*后续一旦Ki不为0，那么因为误差积分已经积累到很大的值了，这就导致积分项疯狂输出，不利于调试*/
	if (p->Ki != 0)					//如果Ki不为0
	{
		p->ErrorInt += p->Error0;	//进行误差积分
	}
	else							//否则
	{
		p->ErrorInt = 0;			//误差积分直接归0
	}
	/*使用位置式PID公式，计算得到输出值*/
	p->Out = p->Kp * p->Error0
		   + p->Ki * p->ErrorInt
		   + p->Kd * (p->Error0 - p->Error1);
	/*输出限幅*/
	if (p->Out > p->OutMax) {p->Out = p->OutMax;}	//限制输出值最大为结构体指定的OutMax
	if (p->Out < p->OutMin) {p->Out = p->OutMin;}	//限制输出值最小为结构体指定的OutMin
}

void Angle_Tweak (void)
{
	AnglePID.Kp = parameter[1][0];
	AnglePID.Ki = parameter[1][1];
	AnglePID.Kd = parameter[1][2];
	
	if (pitch > 60 || pitch < -60)
	{
		RunFlag = 0;
	}
	if (RunFlag)
	{
		AnglePID.Actual = pitch;
		PID_Update(&AnglePID);
		AvePWM = -AnglePID.Out;
		
		LeftPWM = AvePWM + DifPWM / 2;
		RightPWM = AvePWM - DifPWM / 2;
		
		if (LeftPWM > 100) {LeftPWM = 100;} else if (LeftPWM < -100) {LeftPWM = -100;}
		if (RightPWM > 100) {RightPWM = 100;} else if (RightPWM < -100) {RightPWM = -100;}
		
		Set_Motor1(LeftPWM);
		Set_Motor2(RightPWM);
	}else
	{
		Set_Motor1(0);
		Set_Motor2(0);
	}
}

void Speed_Tweak (void)
{
	SpeedPID.Kp = parameter[1][0];
	SpeedPID.Ki = parameter[1][1];
	SpeedPID.Kd = parameter[1][2];
	
	LeftSpeed = encoder_get_count(ENCODER_1) / 44.0 / 0.01 / 30;
	encoder_clear_count(ENCODER_1);
	RightSpeed = encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30;
	encoder_clear_count(ENCODER_2);
	
	AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
	
	if (RunFlag)
	{
		SpeedPID.Actual = AveSpeed;
		PID_Update(&SpeedPID);
		AnglePID.Target = SpeedPID.Out;
	}
	
}

void Turn_Tweak(void)
{
	DifSpeed = LeftSpeed - RightSpeed;
	if (RunFlag)
	{
		TurnPID.Actual = DifSpeed;
		PID_Update(&TurnPID);
		DifPWM = TurnPID.Out;
	}
}