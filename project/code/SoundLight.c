#include "SoundLight.h"
#include "sensor.h"

uint8 LEDstat = 0, Buzzerstat = 0;
static uint16 cnt = 0;
uint8 active = 0;

void SoundLight_Init(void)
{
	gpio_init(LED, GPO, 0,GPO_PUSH_PULL);
	gpio_init(Buzzer, GPO, 0,GPO_PUSH_PULL);
}

void SoundLight_React(void)
{
	LEDstat = gpio_get_level(LED);
	Buzzerstat = gpio_get_level(Buzzer);
	
	if(flag)
	{
		gpio_set_level(LED,0);
		gpio_set_level(Buzzer,1);
		active = 1;cnt = Act_Duration;
		LEDstat = 1,Buzzerstat = 1;
	}
	if(active)
	{
		if(cnt)cnt--;
		else{
		gpio_set_level(LED,1);
		gpio_set_level(Buzzer,0);
		active = 0;
		LEDstat = 0,Buzzerstat = 0;
		}
	}
}
