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
	mpu6050_init();	
	Kalman_Init(&KF, 0.0001f, 0.003f, 0.03f);
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
		BlueTooth_Update();	//蓝牙刷新
		
		/*测试使用*/
		ips200_show_float(0,96,LeftSpeed,4,2);
		ips200_show_float(0,112,RightSpeed,4,2);
		ips200_show_float(0,128,pitch,4,2);
//		BlueSerial_Printf("[plot,%f,%f]", SpeedPID.Target, AveSpeed);
//			BlueSerial_Printf("[plot,%f]", pitch);
		BlueSerial_Printf("[plot,%f,%f]", pitch ,AveSpeed);
	}
}

void pit_handler(void)  //10ms定时中断
{
//	static uint16 count0;
//	count0++;
//	/*10ms*/
//	if(count0>=10)
//	{	
//		count0=0;
	
		Calculate_Attitude();	//姿态解算
		Angle_Tweak();		 		//角度环PID			
	
		//读取编码器（动态滤波控制波动）		
		float speed_filter = 1.0; 
    if(fabs(pitch) < 5 ) { speed_filter = 0.3; } 	 // 静态强滤波
    else { speed_filter = 0.5;}  									 // 动态弱滤波
		LeftSpeed = speed_filter * (encoder_get_count(ENCODER_1) / 44.0 / 0.01 / 30.0 ) + (1-speed_filter) * Last_LeftSpeed;
		RightSpeed = speed_filter * (encoder_get_count(ENCODER_2) / 44.0 / 0.01 / 30.0 ) + (1-speed_filter)* Last_RightSpeed;
		if(Last_LeftSpeed-LeftSpeed>=0.5  || Last_LeftSpeed-LeftSpeed<=-0.5)LeftSpeed=Last_LeftSpeed;
		if(Last_RightSpeed-RightSpeed>=0.5  || Last_RightSpeed-RightSpeed<=-0.5)RightSpeed=Last_RightSpeed;
		encoder_clear_count(ENCODER_1);
		encoder_clear_count(ENCODER_2);	
		
		//计算速度
		Last_LeftSpeed=LeftSpeed;
		Last_RightSpeed=RightSpeed;
		AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
		DifSpeed = LeftSpeed - RightSpeed;
			
	  Speed_Tweak();	//速度环PID
		Turn_Tweak();		//转向环PID
		
		AvePWM = - AnglePID.Out -	SpeedPID.Out;
		DifPWM = TurnPID.Out;	
		
		LeftPWM = AvePWM + DifPWM / 2;
		RightPWM = AvePWM - DifPWM / 2;	
		if (LeftPWM > 100) {LeftPWM = 100;}
		else if (LeftPWM < -100) {LeftPWM = -100;}
		if (RightPWM > 100) {RightPWM = 100;} 
		else if (RightPWM < -100) {RightPWM = -100;}
		
		if(RunFlag){
			Set_Motor1(LeftPWM);
			Set_Motor2(RightPWM);
		}else{
			Set_Motor1(LeftPWM);
			Set_Motor2(RightPWM);			
		}
		
		
//	}	
}
