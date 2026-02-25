#include "Encoder.h"
#include "Motor.h"

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
