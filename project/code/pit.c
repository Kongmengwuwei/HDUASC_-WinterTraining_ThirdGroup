#include "pit.h"

/*PIT初始化*/
void Pit_Init(void)
{
	pit_ms_init(PIT_CH, 1);						//间隔1ms
	interrupt_set_priority(PIT_PRIORITY, 0);		//中断优先级
}
