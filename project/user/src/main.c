#include "zf_common_headfile.h"

#include "pit.h"
#include "menu.h"
#include "buzzer.h"
#include "flash.h"
#include "mpu6050.h"
#include "Motor.h"

int16 encoder_data_1 = 0;
int16 encoder_data_2 = 0;

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();					// 调试端口初始化
	
	flash_init();
	key_init(10);
	Menu_Init();
	Buzzer_Init();
	Mpu6050_Init();
//	Motor_Init();
	Pit_Init();
	
	interrupt_global_enable(0);
	
	while(1)
	{
		Menu_Update();
		Mpu6050_Show();	}
}

void pit_handler (void)			//10ms中断
{
	key_scanner();
	Mpu6050_Read();
	
//	encoder_data_1 = encoder_get_count(ENCODER_1);                              // 获取编码器计数
//  encoder_clear_count(ENCODER_1);                                             // 清空编码器计数

//  encoder_data_2 = encoder_get_count(ENCODER_2);                              // 获取编码器计数
//  encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
}

