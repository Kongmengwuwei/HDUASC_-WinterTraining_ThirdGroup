#include "zf_common_headfile.h"

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
		Mpu6050_Show();	}
}

void pit_handler (void)			//10ms中断
{
	key_scanner();
	Mpu6050_Read();
}

