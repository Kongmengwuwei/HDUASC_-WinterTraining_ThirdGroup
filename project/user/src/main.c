#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
#include "pit.h"
#include "key_handler.h"
#include "sensor.h"
#include "mpu6050.h"
#include "Motor.h"
#include "bluetooth.h"
#include "menu.h"
#include "flash.h"
#include "pid.h"

int main(void)
{
	/*初始化部分*/
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();										// 调试端口初始化
	system_delay_ms(300);	
	key_init(10);
	key_handler_init();	
	Sensor_Init();
	flash_init();
	Bluetooth_Init();
	Menu_Init();
	Mpu6050_Init();
	Motor_Init();
	Pit_Init();
	interrupt_global_enable(0);
	
	/*测试使用*/
//	Set_Motor1(-50);
//	Set_Motor2(-50);

	while(1)
	{
		key_event_scan();		//按键检测
		Menu_Update();			//菜单刷新
		Mpu6050_Show();			//陀螺仪数据
		BlueTooth_Update();	//蓝牙刷新
		
		/*测试使用*/
		ips200_show_float(0,96,LeftSpeed,4,2);
		ips200_show_float(0,112,RightSpeed,4,2);
//		BlueSerial_Printf("[plot,%f,%f]", SpeedPID.Target, AveSpeed);
		BlueSerial_Printf("[plot,%f,%f,%f]", LeftSpeed, RightSpeed,AveSpeed);
	}
}

void pit_handler(void)  //1ms定时中断
{
	static uint16 count;
	count++;
	
	
	/*1ms*/
	Mpu6050_Read();		//读取陀螺仪
	Angle_Tweak();		//角度环PID
	
	
	/*10ms*/
	if(count>=10)
	{
		count=0;
		//读取编码器（滤波控制波动）
		LeftSpeed = 0.3 * (encoder_get_count(ENCODER_1) / 44.0 / 0.01 / 30.0 ) + 0.7 * Last_LeftSpeed;
		RightSpeed = 0.3 * (encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30.0 ) + 0.7* Last_RightSpeed;
		if(Last_LeftSpeed-LeftSpeed>=0.3  || Last_LeftSpeed-LeftSpeed<=-0.3)LeftSpeed=Last_LeftSpeed;
		if(Last_RightSpeed-RightSpeed>=0.3  || Last_RightSpeed-RightSpeed<=-0.3)RightSpeed=Last_RightSpeed;
		encoder_clear_count(ENCODER_1);
		encoder_clear_count(ENCODER_2);	
		
		//计算速度
		Last_LeftSpeed=LeftSpeed;
		Last_RightSpeed=RightSpeed;
		AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
		DifSpeed = LeftSpeed - RightSpeed;
			
//	  Speed_Tweak();	//速度环PID
//		Turn_Tweak();		//转向环PID
	}
}
