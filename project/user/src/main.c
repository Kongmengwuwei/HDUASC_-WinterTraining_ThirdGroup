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

	Set_Motor1(50);	
	Set_Motor2(50);
	
	while(1)
	{
		key_event_scan();
		Menu_Update();
		Mpu6050_Show();
	
		ips200_show_float(0,96,LeftSpeed,4,2);
	  ips200_show_float(0,112,RightSpeed,4,2);
		ips200_show_float(80,112,AnglePID.Target,4,2);
		
		BlueTooth_Update();
		BlueSerial_Printf("[plot,%f,%f]", SpeedPID.Target, AveSpeed);
	}
}

void pit_handler(void)
{
	static uint8 count0,count1;

//	Mpu6050_Read();
//	Angle_Tweak();
//		
	count1++;
	if(count1>=10)
	{
		Speed_Tweak();
	}
}

