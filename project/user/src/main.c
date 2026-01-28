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
// int16 encoder_data_1 = 0;
// int16 encoder_data_2 = 0;

PID_t AnglePID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0,
	
	.OutMax = 100,
	.OutMin = -100,
};

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M); 
	debug_init();

	Buzzer_Init();
	flash_init();
	key_init(10);
	Menu_Init();
	Mpu6050_Init();
	//Motor_Init();
	Pit_Init();

	interrupt_global_enable(0);
	while (1)
	{
		Menu_Update();
		Mpu6050_Show();
	}
}

void pit_handler(void)
{
	key_scanner();
	Mpu6050_Read();
	
	if (pitch > 50 || pitch < -50)
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
		
		Set_Moter1(LeftPWM);
		Set_Moter2(RightPWM);
	}
	else
	{
		Set_Moter1(0);
		Set_Moter2(0);
	}
	//	encoder_data_1 = encoder_get_count(ENCODER_1);                              // ?????????????
	//  encoder_clear_count(ENCODER_1);                                             // ????????????

	//  encoder_data_2 = encoder_get_count(ENCODER_2);                              // ?????????????
	//  encoder_clear_count(ENCODER_2);                                             // ????????????
}
