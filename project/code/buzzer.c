#include "buzzer.h"

void Buzzer_Init(void)
{
	gpio_init(B11,GPO,0,GPO_PUSH_PULL);
}
