#include "pit.h"

/*PIT初始化*/
void Pit_Init(void)
{
	pit_ms_init(PIT_CH, 10);						//间隔10ms
	interrupt_set_priority(PIT_PRIORITY, 0);		//中断优先级
}
