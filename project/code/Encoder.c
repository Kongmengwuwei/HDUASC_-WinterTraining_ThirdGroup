#include "Encoder.h"
#include "Motor.h"
#include "mpu6050.h"
#include <math.h>

float LeftSpeed = 0, RightSpeed = 0, Last_LeftSpeed= 0 , Last_RightSpeed = 0; //左轮速度 右轮速度 上次左轮速度 上次右轮速度
float AveSpeed = 0, DifSpeed = 0;	//平均速度	差速

float leftcount,rightcount;

/*编码器初始化*/
void Encoder_Init(void)
{
	encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);             // 初始化编码器模块与引脚 正交解码编码器模式
	encoder_quad_init(ENCODER_2, ENCODER_2_A, ENCODER_2_B);             // 初始化编码器模块与引脚 正交解码编码器模式	
}

/*读取并清除编码器数据*/
void Read_Encoder(void)
{
	leftcount=encoder_get_count(ENCODER_1);
	encoder_clear_count(ENCODER_1);
	rightcount=encoder_get_count(ENCODER_2);	
	encoder_clear_count(ENCODER_2);		
}

void Speed_Get(void)
{
		Read_Encoder();		
		float speed_filter = 1.0; 
    if(fabs(pitch) < 3.0f ) { speed_filter = 0.2f; } 	// 静态强滤波
    else { speed_filter = 0.4f;}  									 	// 动态弱滤波
		LeftSpeed = speed_filter * (leftcount / 44.0 / 0.005 / 30.0 ) + (1-speed_filter) * Last_LeftSpeed;
		RightSpeed = speed_filter * (rightcount  / 44.0 / 0.005 / 30.0) + (1-speed_filter)* Last_RightSpeed;
		Last_LeftSpeed=LeftSpeed;
		Last_RightSpeed=RightSpeed;
		if(Last_LeftSpeed-LeftSpeed>=0.5 || Last_LeftSpeed-LeftSpeed<=-0.5)LeftSpeed=Last_LeftSpeed;
		if(Last_RightSpeed-RightSpeed>=0.5 || Last_RightSpeed-RightSpeed<=-0.5)RightSpeed=Last_RightSpeed;	
	
}	
	