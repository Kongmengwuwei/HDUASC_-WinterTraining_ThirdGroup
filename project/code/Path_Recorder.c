#include "path_recorder.h"
#include "pid.h"
#include "mpu6050.h"
#include "Encoder.h"
#include <string.h>
#include <math.h>

extern uint8 Recorder_Flag, Tracking_Flag;
int8 cnt;
float Left,Right,Yaw;

PathManager_t path_manager;
float Navigation_Speed, Navigation_Turn;

// 路径跟踪初始化
void PathTracking_Init(void)
{
    memset(&path_manager, 0, sizeof(PathManager_t));
}

// 记录路径点
void Record_PathPoint(void)
{ 
	Left += LeftSpeed;
	Right += RightSpeed;
	Yaw += yaw;
	cnt++;
	if(cnt>=8){
		cnt=0;
		
		PathPoint_t *p = &path_manager.points[path_manager.count];

		p->left_speed = Left / 8.0;
		p->right_speed = Right / 8.0;			
		p->yaw = Yaw / 8.0;
		Left=0;
		Right=0;
		Yaw=0;
		path_manager.count++;
	}
}

// 导航计算
void Navigation_Calculate(void)
{  
	if(Tracking_Flag==1)
	{
		
		PathPoint_t *target = &path_manager.points[path_manager.current_index];
		
		// 计算距离偏差
		float left_diff = target->left_speed - LeftSpeed;
		float right_diff = target->right_speed - RightSpeed;
		
		// 计算角度偏差
		float yaw_diff = target->yaw - yaw;
		
		// 计算目标速度
		Navigation_Speed = (left_diff + right_diff) / 2.0f;
		
		// 计算转向控制量
		Navigation_Turn = yaw_diff;     
		
		// 检查是否到达目标点
		path_manager.current_index++;					
		if(path_manager.current_index >= path_manager.count)
		{
				Navigation_Speed=0;
				Navigation_Turn=0;
				PID_Init(&GyroPID);				
				PID_Init(&AnglePID);
				PID_Init(&SpeedPID);
				PID_Init(&TurnPID);
				Tracking_Flag=0;
		}
	}
}
