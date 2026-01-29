#include "zf_common_headfile.h"

#include "pit.h"
#include "menu.h"
#include "buzzer.h"
#include "flash.h"
#include "mpu6050.h"
#include "Motor.h"
#include "pid.h"

uint8 RunFlag;
int16 LeftPWM, RightPWM;
int16 AvePWM, DifPWM;
float LeftSpeed, RightSpeed;
float AveSpeed, DifSpeed;
int16 encoder_data_1 = 0;
int16 encoder_data_2 = 0;

PID_t AnglePID = {
	.Kp = 6,
	.Ki = 0.01,
	.Kd =6,
	
	.OutMax = 100,
	.OutMin = -100,
};

PID_t SpeedPID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,
	
	.OutMax = 20,
	.OutMin = -20,
	
	.ErrorIntMax = 150,
	.ErrorIntMin = -150,
};

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M); 
	debug_init();
	
	 system_delay_ms(300); 

	Buzzer_Init();
	flash_init();
	key_init(10);
	Menu_Init();
	Mpu6050_Init();
	Motor_Init();
	Pit_Init();

	interrupt_global_enable(0);
	
	RunFlag=1;
	
	while (1)
	{
		key_scanner();
		Menu_Update();
		Mpu6050_Show();
		ips200_show_int(0,128,LeftSpeed,5);
		ips200_show_int(0,144,RightSpeed,5);	  
//		printf("ENCODER_1 counter \t%d .\r\n", encoder_data_1);                 // 输出编码器计数信息
//    printf("ENCODER_2 counter \t%d .\r\n", encoder_data_2);                 // 输出编码器计数信息  
      
	}
}

void pit_handler(void)
{
	static uint16 count0;
	
	Mpu6050_Read();
	
	//倾角过大自动停止
	if (pitch > 60 || pitch < -60)
	{
		RunFlag = 0;
	}
	//角度环PID
	if (RunFlag)
	{
		AnglePID.Actual = pitch + 3.5;
		PID_Update(&AnglePID);
		AvePWM = -AnglePID.Out;
		
		LeftPWM = AvePWM + DifPWM / 2;
		RightPWM = AvePWM - DifPWM / 2;
		
		if (LeftPWM > 100) {LeftPWM = 100;} else if (LeftPWM < -100) {LeftPWM = -100;}
		if (RightPWM > 100) {RightPWM = 100;} else if (RightPWM < -100) {RightPWM = -100;}
		
		Set_Moter1(LeftPWM);
		Set_Moter2(RightPWM);
	}else
	{
		Set_Moter1(0);
		Set_Moter2(0);
	}

	
//	count0++;						
//	if(count0>=10)
//	{
//		count0=0;
//		
//		//速度环PID
//		LeftSpeed = encoder_get_count(ENCODER_1) / 44.0 / 0.01 / 30;
//		RightSpeed = encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30;
//		
//		AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
//		DifSpeed = LeftSpeed - RightSpeed;
//	
//		if (RunFlag)
//		{
//			SpeedPID.Actual = AveSpeed;
//			PID_Update(&SpeedPID);
//			AnglePID.Target = SpeedPID.Out;
//		}
//	}
	  
}
