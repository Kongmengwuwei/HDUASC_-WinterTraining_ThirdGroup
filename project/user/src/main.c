#include "zf_common_headfile.h"
#include "pit.h"
#include "key_handler.h"
#include "sensor.h"
#include "mpu6050.h"
#include "Motor.h"
#include "Encoder.h"
#include "bluetooth.h"
#include "SoundLight.h"
#include "menu.h"
#include "flash.h"
#include "pid.h"
#include "FollowRoute.h"
#include "Path_Recorder.h"

uint8 Mode = 1, LastMode = 1; 					//发车模式
uint8 flag1=0, flag2=0;		// 杂项标志位
uint8 RunFlag = 0; 				//电机运行标志位
uint8 SoundLightFlag = 0; //声光标志位
uint8 Recorder_Flag=0, Tracking_Flag=0;		//路线记忆模式标志位

int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);
    debug_init();
    system_delay_ms(300);
    
		key_init(10);										//按键初始化
		key_handler_init();							
		SoundLight_Init();							//声光板初始化
		Sensor_Init();									//循迹传感器初始化
		flash_init();                   //Flash初始化
		Bluetooth_Init();								//蓝牙初始化
		Menu_Init();										//菜单初始化
		mpu6050_init();									//陀螺仪初始化
		Kalman_Init(&KF, 0.001f, 0.003f, 0.05f);		//卡尔曼姿态解算初始化
		Encoder_Init();        					//编码器初始化
		Motor_Init();										//电机初始化
		Pit_Init();											//定时中断初始化
		interrupt_global_enable(0);
	
	 /*主循环*/
    while(1)
    {
				Sensor_Check();			//循迹检测
				key_event_scan();		//按键检测
				Menu_Update();			//菜单刷新
				BlueTooth_Update();	//蓝牙接收发送
        
				//ips200屏幕显示运行信息
        ips200_show_string(0, 128, "Mode:");
        ips200_show_uint(64, 128, Mode, 1);
        ips200_show_string(0, 144, "RunFlag:");
        ips200_show_uint(64, 144, RunFlag, 1);    
        ips200_show_int(0, 280, flag_FollowRoute, 1);

        
				//蓝牙发送信息
				BlueSerial_Printf("[plot,%f,%f]", LeftSpeed, RightSpeed);
//        BlueSerial_Printf("[plot,%d,%d,%d,%d]", flag_FollowRoute, cnt_slowdown, cnt_speedup,cnt_FollowLine);
		
				// 在主循环显示调试信息
				ips200_show_string(0, 192, "Yaw:");
				ips200_show_float(40, 192, yaw, 3, 2);
				ips200_show_string(0, 208, "Pitch:");
				ips200_show_float(56, 208, pitch, 3, 2);
    }
}

void pit_handler(void)	//1ms定时中断
{
    static uint16 count5ms, count100ms ;
    count5ms++;
		count100ms++;
    
		/* 100ms定时中断部分 */
    if(count100ms >= 100)
    {
        count100ms = 0;
				
				//声光提示
				if(!SoundLightFlag){
					SoundLight_Off();
				}
				if(SoundLightFlag){
					SoundLight_On();
					SoundLightFlag = 0;
				}

    }
		
    /* 5ms定时中断部分 */
    if(count5ms >= 5)
    {
        count5ms = 0;
			
				//模式切换时重置数据
				if(LastMode != Mode)
				{
					yaw = 0;
					SpeedPID.ErrorIntMax = 30;
					SpeedPID.ErrorIntMin = -30;
					flag_FollowRoute = 1;
					PID_Init(&GyroPID);
					PID_Init(&AnglePID);
					PID_Init(&SpeedPID);
					PID_Init(&TurnPID);
					PID_Init(&TracePID);
					PID_Init(&YAWPID);	
				}			
				LastMode = Mode;
			
        //获得左右轮速度
        Speed_Get();
				//计算速度
        AveSpeed = (LeftSpeed + RightSpeed) / 2.0;
        DifSpeed = LeftSpeed - RightSpeed;
        
				//模式1：原地
				if(Mode == 1)
				{
					SpeedPID.ErrorIntMax = 1000;
					SpeedPID.ErrorIntMin = -1000;
				}
				
			  // 模式2、3：循迹
        if(Mode == 2 || Mode == 3)
        {
            Follow_Route();
            Follow_Route_Tweak();
        }
				
        // 模式4：记录
        if(Mode == 4 && Recorder_Flag)
        {
            Record_PathPoint();
        }
        
        // 模式4：回放
        if(Mode == 4 && Tracking_Flag)
        {
            Navigation_Calculate();
        }
        
        // 速度环
        Speed_Tweak();
        
        // 转向环
        Turn_Tweak();

    }
    
    /* 1ms定时中断部分 */
		//俯仰角过大自动停机
    if(pitch > 50 || pitch < -50)
    {
        RunFlag = 0;
    }
		
    //姿态解算
    Calculate_Attitude();
		
		//角度环PID
    Angle_Tweak();
		
		//角速度环PID	
    Gyro_Tweak();
    
		//计算并应用最终输出于电机
    if(RunFlag)
    {
        LeftPWM = AvePWM + DifPWM / 2;
        RightPWM = AvePWM - DifPWM / 2;
        
        if(LeftPWM > 10000) LeftPWM = 10000;
        else if(LeftPWM < -10000) LeftPWM = -10000;
        if(RightPWM > 10000) RightPWM = 10000;
        else if(RightPWM < -10000) RightPWM = -10000;
        
        Set_Motor1(LeftPWM);
        Set_Motor2(RightPWM);
    }
    else
    {
        Set_Motor1(0);
        Set_Motor2(0);
    }
}
