#ifndef __PIT_H
#define __PIT_H

#include "zf_common_headfile.h"

#define PIT_CH					(PIT_CH0 )										// 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY			(PIT_IRQn)										// 对应周期中断的中断编号 

void Pit_Init(void);

#endif
