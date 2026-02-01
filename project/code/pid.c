#include "pid.h"
#include "mpu6050.h"
#include "Motor.h"
#include "sensor.h"
#include "menu.h"

uint8 RunFlag = 1; 	//运行标志位
int16 LeftPWM = 0, RightPWM = 0;	//左PWM 右PWM
int16 AvePWM = 0, DifPWM = 0;			//平均PWM 差PWM
float LeftSpeed = 0, RightSpeed = 0, Last_LeftSpeed= 0 , Last_RightSpeed = 0; //左轮速度 右轮速度 上次左轮速度 上次右轮速度（编码器读得）
float AveSpeed = 0, DifSpeed = 0;	//平均速度	差速

PID_t AnglePID = {
	.OutMax = 100,
	.OutMin = -100,
	
	.OutOffset=3,
	.ErrorIntMax=1000,
	.ErrorIntMin=-1000,
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
PID_t TracePID = {
	.OutMax = 0,
	.OutMin = 0,
};

/*PID参数清除*/
void PID_Init(PID_t *p)
{
	p->Target = 0;
	p->Actual = 0;
	p->Actual1 = 0;
	p->Out = 0;
	p->Error0 = 0;
	p->Error1 = 0;
	p->ErrorInt = 0;
}
/*PID计算函数*/
void PID_Update(PID_t *p)
{
	p->Error1 = p->Error0;
	p->Error0 = p->Target - p->Actual;
	
	if (p->Ki != 0)
	{
		p->ErrorInt += p->Error0;
		
		//积分限幅
		if (p->ErrorInt > p->ErrorIntMax) {p->ErrorInt = p->ErrorIntMax;}
		if (p->ErrorInt < p->ErrorIntMin) {p->ErrorInt = p->ErrorIntMin;}
	}
	else
	{
		p->ErrorInt = 0;
	}
	
	p->Out = p->Kp * p->Error0
		   + p->Ki * p->ErrorInt
//		   + p->Kd * (p->Error0 - p->Error1);
		   - p->Kd * (p->Actual - p->Actual1);	//微分先行
	
	//输出偏移
	if (p->Out > 0) {p->Out += p->OutOffset;}
	if (p->Out < 0) {p->Out -= p->OutOffset;}
	//输出限幅
	if (p->Out > p->OutMax) {p->Out = p->OutMax;}
	if (p->Out < p->OutMin) {p->Out = p->OutMin;}
	
	p->Actual1 = p->Actual;
}

void Angle_Tweak (void)		//角度环PID（结果输出给电机）
{
	AnglePID.Kp = parameter[1][0];
	AnglePID.Ki = parameter[1][1];
	AnglePID.Kd = parameter[1][2];
	
	//俯仰角过大自动停机
	if (pitch > 30 || pitch < -30) 
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
		
		if (LeftPWM > 100) {LeftPWM = 100;}
		else if (LeftPWM < -100) {LeftPWM = -100;}
		if (RightPWM > 100) {RightPWM = 100;} 
		else if (RightPWM < -100) {RightPWM = -100;}
		
		Set_Motor1(LeftPWM);
		Set_Motor2(RightPWM);
	}else
	{
		Set_Motor1(0);
		Set_Motor2(0);
	}
}

void Speed_Tweak (void)		//速度环PID（结果输出给角度环）
{
	SpeedPID.Kp = parameter[2][0];
	SpeedPID.Ki = parameter[2][1];
	SpeedPID.Kd = parameter[2][2];
	
	if (RunFlag)
	{
		SpeedPID.Actual = AveSpeed;
		PID_Update(&SpeedPID);
		AnglePID.Target = SpeedPID.Out;
	}
}

void Turn_Tweak(void)		//转向环PID（结果输出给角度环）
{
	TurnPID.Kp = parameter[3][0];
	TurnPID.Ki = parameter[3][1];
	TurnPID.Kd = parameter[3][2];
	
	if (RunFlag)
	{
		TurnPID.Actual = DifSpeed;
		PID_Update(&TurnPID);
		DifPWM = TurnPID.Out;
	}
}

void Trace_Tweak(void)		//转向环PID（结果输出给角度环）
{
	TurnPID.Kp = parameter[4][0];
	TurnPID.Ki = parameter[4][1];
	TurnPID.Kd = parameter[4][2];
	
	if (RunFlag)
	{
		TracePID.Actual = Sensor_Check();
		PID_Update(&TracePID);
		TurnPID.Target = TracePID.Out;
	}
}