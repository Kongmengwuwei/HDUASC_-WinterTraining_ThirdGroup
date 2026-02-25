#include "zf_common_headfile.h"
// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完
#include "pit.h"
#include "key_handler.h"
#include "sensor.h"
#include "mpu6050.h"
#include "Motor.h"
#include "Encoder.h"
#include "bluetooth.h"
#include "menu.h"
#include "flash.h"
#include "pid.h"

uint8 RunFlag = 1; 	//电机运行标志位
uint8 Mode = 1; 		//发车模式

int main(void)
{
	/*初始化部分*/
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();										// 调试端口初始化
	system_delay_ms(300);	

	key_init(10);										//按键初始化
	key_handler_init();							
	Sensor_Init();									//循迹传感器初始化
	flash_init();                   //Flash初始化
	Bluetooth_Init();								//蓝牙初始化
	Menu_Init();										//菜单初始化
	mpu6050_init();									//陀螺仪初始化
	Kalman_Init(&KF, 0.0005f, 0.003f, 0.1f);		//卡尔曼姿态解算初始化
	Encoder_Init();        					//编码器初始化
	Motor_Init();										//电机初始化
	Pit_Init();											//定时中断初始化
	interrupt_global_enable(0);
	
	/*测试使用*/
//	Set_Motor1(-50);
//	Set_Motor2(-50);

	/*主循环*/
	while(1)
	{
		key_event_scan();		//按键检测
		Menu_Update();			//菜单刷新
		BlueTooth_Update();	//蓝牙接收发送
		
		/*测试使用*/
//		ips200_show_float(0,96,LeftSpeed,4,2);
//		ips200_show_float(0,112,RightSpeed,4,2);
//		ips200_show_float(0,128,pitch,4,2);
//		ips200_show_float(0,144,gyro_pitch,4,2);
		
//		BlueSerial_Printf("[plot,%f,%f,%f]", LeftSpeed, RightSpeed, DifPWM);
//		BlueSerial_Printf("[plot,%f,%f]", pitch ,gyro_pitch);
	}
}

void pit_handler(void)  //1ms定时中断
{
	static uint16 count0;
	count0++;			 				
	/*以下为5ms定时中断*/
	if(count0>=5)
	{
		count0=0;
	
		//读取编码器（动态滤波限幅控制波动）		
		Read_Encoder();		
		float speed_filter = 1.0; 
    if(fabs(pitch) < 3.0f ) { speed_filter = 0.2f; } 	// 静态强滤波
    else { speed_filter = 0.6f;}  									 	// 动态弱滤波
		LeftSpeed = speed_filter * (leftcount / 44.0 / 0.005 / 30.0 ) + (1-speed_filter) * Last_LeftSpeed;
		RightSpeed = speed_filter * (rightcount  / 44.0 / 0.005 / 30.0) + (1-speed_filter)* Last_RightSpeed;
		Last_LeftSpeed=LeftSpeed;
		Last_RightSpeed=RightSpeed;
		if(Last_LeftSpeed-LeftSpeed>=0.5 || Last_LeftSpeed-LeftSpeed<=-0.5)LeftSpeed=Last_LeftSpeed;
		if(Last_RightSpeed-RightSpeed>=0.5 || Last_RightSpeed-RightSpeed<=-0.5)RightSpeed=Last_RightSpeed;
		
		//计算速度
		AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
		DifSpeed = LeftSpeed - RightSpeed;
		
		//速度环PID	
	  Speed_Tweak();
		
		//转向环PID	
		Turn_Tweak();
	}
	/*以上为5ms定时中断*/	

	
	//俯仰角过大自动停机
	if (pitch > 50 || pitch < -50) 
	{
		RunFlag = 0;
	}
	
	//姿态解算
	Calculate_Attitude();
	
	//角度环PID
	Angle_Tweak();
	
	//角速度环PID	
	Gyro_Tweak();	
	
	//应用最终输出于电机
	if (RunFlag)
	{	
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
