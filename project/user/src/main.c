#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
#include "pit.h"
#include "key_handler.h"
#include "menu.h"
#include "flash.h"
#include "mpu6050.h"
#include "Motor.h"
#include "pid.h"
#include "bluetooth.h"

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();					// 调试端口初始化
	system_delay_ms(300);
	
	Bluetooth_Init();
	Motor_Init();
	flash_init();
	key_init(10);
	key_handler_init();
	Menu_Init();
	Mpu6050_Init();
	Pit_Init();
	
	interrupt_global_enable(0);
	
//	Set_Motor1(50);
//	Set_Motor2(50);

	while(1)
	{
		key_event_scan();
		Menu_Update();
		Mpu6050_Show();

		ips200_show_float(0,96,LeftSpeed,4,2);
		ips200_show_float(0,112,RightSpeed,4,2);
//		ips200_show_float(80,112,AnglePID.Target,4,2);
		
		BlueTooth_Update();
//		BlueSerial_Printf("[plot,%f,%f]", SpeedPID.Target, AveSpeed);
		BlueSerial_Printf("[plot,%f,%f,%f]", LeftSpeed, RightSpeed,AveSpeed);
	}
}

void pit_handler(void)
{
	static uint16 count0;
	count0++;
	
	Mpu6050_Read();
	Angle_Tweak();
	
	if(count0>=10)
	{
		count0=0;
		LeftSpeed = 0.3 * (encoder_get_count(ENCODER_1) / 44.0 / 0.01 / 30.0 ) + 0.7 * Last_LeftSpeed;
		RightSpeed = 0.3 * (encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30.0 ) + 0.7* Last_RightSpeed;
		if(Last_LeftSpeed-LeftSpeed>=0.3  || Last_LeftSpeed-LeftSpeed<=-0.3)LeftSpeed=Last_LeftSpeed;
		if(Last_RightSpeed-RightSpeed>=0.3  || Last_RightSpeed-RightSpeed<=-0.3)RightSpeed=Last_RightSpeed;
		encoder_clear_count(ENCODER_1);
		encoder_clear_count(ENCODER_2);		
		Last_LeftSpeed=LeftSpeed;
		Last_RightSpeed=RightSpeed;
		AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
		DifSpeed = LeftSpeed - RightSpeed;
	
	  Speed_Tweak();
	}
}

