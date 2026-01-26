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

int main(void)
{
	clock_init(SYSTEM_CLOCK_600M);	// 不可删除
	debug_init();					// 调试端口初始化
	
	Pit_Init();
	flash_init();
	
	key_init(10);
	Menu_Init();
	Buzzer_Init();
	Mpu6050_Init();
	
	while(1)
	{
		Menu_Update();
		Mpu6050_Show();
	}
}

void pit_handler (void)
{
	key_scanner();
	Mpu6050_Read();
}

