#include "path_recorder.h"
#include "pid.h"
#include "mpu6050.h"
#include "Encoder.h"
#include <string.h>
#include <math.h>

extern uint8 Recorder_Flag, Tracking_Flag;
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
		
	PathPoint_t *p = &path_manager.points[path_manager.count];

	p->left_speed = LeftSpeed;
	p->right_speed = RightSpeed;		
	p->yaw = yaw;
	path_manager.count++;
}

// 导航计算
void Navigation_Calculate(void)
{  
	if(Tracking_Flag==1)
	{
		
		PathPoint_t *target = &path_manager.points[path_manager.current_index];
		
		// 计算目标速度
		Navigation_Speed = (target->left_speed + target->right_speed) / 2.0f;
		
		// 计算转向控制量
		Navigation_Turn = target->yaw;     
		
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
