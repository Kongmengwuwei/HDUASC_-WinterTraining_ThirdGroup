#include "buzzer.h"

/*·äÃùÆ÷³õÊ¼»¯*/
void Buzzer_Init(void)
{
	gpio_init(B11,GPO,0,GPO_PUSH_PULL);					//ÆôÓÃGPIO
}
