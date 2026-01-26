#include "zf_common_headfile.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库移植用空工程
#include "pit.h"
#include "menu.h"
float ax,ay,az,gx,gy,gz;
int main(void)
{
    clock_init(SYSTEM_CLOCK_600M);  // 不可删除
    debug_init();                   // 调试端口初始化
	key_init(10);
	Menu_Init();
	Pit_Init();
	gpio_init(B14,GPI,1,GPI_PULL_DOWN);
	gpio_init(B11,GPO,0,GPO_PUSH_PULL);
    while(1)
    {
//		Menu_Update();
//		if(gpio_get_level(B14))
//			gpio_set_level(B11,1);
//		else
//			gpio_set_level(B11,0);
		if(!mpu6050_init())
		{
			ax = mpu6050_acc_transition(mpu6050_acc_x);ay = mpu6050_acc_transition(mpu6050_acc_y);az = mpu6050_acc_transition(mpu6050_acc_z);
			gx = mpu6050_gyro_transition(mpu6050_gyro_x);gy = mpu6050_gyro_transition(mpu6050_gyro_y);gz = mpu6050_gyro_transition(mpu6050_gyro_z);
			ips200_show_float(0,48,ax,3,1);ips200_show_float(56,48,ay,3,1);ips200_show_float(112,48,az,3,1);
			ips200_show_float(0,64,gx,3,1);ips200_show_float(56,64,gy,3,1);ips200_show_float(112,64,gz,3,1);
		}
    }
}
void pit_handler (void)
{
	key_scanner();
	mpu6050_get_acc();
	mpu6050_get_gyro();
}

