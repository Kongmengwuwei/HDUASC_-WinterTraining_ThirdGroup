#include "pid.h"
#include "mpu6050.h"
#include "Motor.h"
#include "menu.h"

uint8 RunFlag = 1;
int16 LeftPWM = 0, RightPWM = 0;
int16 AvePWM = 0, DifPWM = 0;
int32 LeftEncoder=0, Last_LeftEncoder=0, RightEncoder=0,Last_RightEncoder=0;
float LeftSpeed = 0, RightSpeed = 0;
float aveSpeed = 0,  AveSpeed = 0, Last_AveSpeed = 0,DifSpeed = 0;

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

void I_limit(PID_t *p,int max)
{
	if(p->ErrorInt>max)  p->ErrorInt=max;
	if(p->ErrorInt<-max) p->ErrorInt=-max;
}

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
	p->Error0 = p->Target - p->Actual;		//获取本次误差
	
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
	AnglePID.Kp = pidnum[0][0][0];
	AnglePID.Ki = pidnum[0][0][1];
	AnglePID.Kd = pidnum[0][0][2];
	
	if (pitch > 60 || pitch < -60)
	{
		RunFlag = 0;
	}
	if (RunFlag)
	{
		AnglePID.Actual = pitch-2;
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
	SpeedPID.Kp = pidnum[0][1][0];
	SpeedPID.Ki = pidnum[0][1][1];
	SpeedPID.Kd = pidnum[0][1][2];
	
//	LeftSpeed = encoder_get_count(ENCODER_1) / 44.0 / 0.01  / 30.0;	
//	encoder_clear_count(ENCODER_1);
//	RightSpeed = encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30.0;
//	encoder_clear_count(ENCODER_2);
	LeftEncoder=encoder_get_count(ENCODER_1) ;
	LeftSpeed=(LeftEncoder-Last_LeftEncoder);	
	Last_LeftEncoder=LeftEncoder;
	RightEncoder=encoder_get_count(ENCODER_2) ;
	RightSpeed=(RightEncoder-Last_RightEncoder);	
	Last_RightEncoder=RightEncoder;
	
	AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
	
//	AveSpeed = 0.5*aveSpeed + (1-0.5)*Last_AveSpeed;  //滤波																																																														
//	Last_AveSpeed = AveSpeed;
	
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