#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程

#include "pit.h"
#include "menu.h"
#include "buzzer.h"
#include "flash.h"
#include "mpu6050.h"
#include "Motor.h"

//int16 encoder_data_1 = 0;
//int16 encoder_data_2 = 0;

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();					// 调试端口初始化
	
	Buzzer_Init();
//	Motor_Init();
	flash_init();
	key_init(10);
	Menu_Init();
	
	Mpu6050_Init();
	Pit_Init();
	
	interrupt_global_enable(0);
	while(1)
	{
		Menu_Update();
		Mpu6050_Show();
	}
}

void pit_handler (void)			//定时中断
{
	key_scanner();
	Mpu6050_Read();
	
//	encoder_data_1 = encoder_get_count(ENCODER_1);                              // 获取编码器计数
//  encoder_clear_count(ENCODER_1);                                             // 清空编码器计数

//  encoder_data_2 = encoder_get_count(ENCODER_2);                              // 获取编码器计数
//  encoder_clear_count(ENCODER_2);                                             // 清空编码器计数
}

