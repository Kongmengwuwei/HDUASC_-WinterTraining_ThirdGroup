#include "pit.h"
uint8 pit_state = 0;
void Pit_Init(void)
{
	pit_ms_init(PIT_CH, 10);
    interrupt_set_priority(PIT_PRIORITY, 0);
}
