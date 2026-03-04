#include "SoundLight.h"

/*初始化声光对应引脚*/
void SoundLight_Init(void)
{
	gpio_init(SoundLight, GPO, 0, GPO_PUSH_PULL);
	gpio_init(BEEP, GPO, GPIO_LOW, GPO_PUSH_PULL);
}

/*启动声光提示*/
void SoundLight_On(void)
{
	gpio_set_level(SoundLight,GPIO_HIGH);
  gpio_set_level(BEEP, GPIO_HIGH);
}

/*关闭声光提示*/
void SoundLight_Off(void)
{
	gpio_set_level(SoundLight,GPIO_LOW);
	gpio_set_level(BEEP, GPIO_LOW);
}