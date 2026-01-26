#include "pit.h"

void Pit_Init(void)
{
	pit_ms_init(PIT_CH, 10);
	interrupt_set_priority(PIT_PRIORITY, 0);
}
