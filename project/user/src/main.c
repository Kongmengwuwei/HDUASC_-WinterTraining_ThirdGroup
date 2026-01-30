#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
#include "pit.h"
#include "key_handler.h"
#include "menu.h"
#include "SoundLight.h"
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
	
	while(1)
	{
		key_event_scan();
		Menu_Update();
		Mpu6050_Show();
		
		BlueTooth_Update();ips200_show_int(0,128,(int8)SpeedPID.Kd,5);
	}
}

void pit_handler(void)
{
	Mpu6050_Read();
	
	Angle_Tweak();
	Speed_Tweak();
}

