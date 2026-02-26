#ifndef __FOLLOWROUTE_H
#define __FOLLOWROUTE_H

#include "zf_common_headfile.h"

 
 
extern volatile int flag;                                  //flag状态位：0-暂停；1-AB；2-BC；3-CD；4-DA；

extern volatile int mode;                                  //mode状态位：0-暂停；

extern uint8 RunFlag = 1; 	                               //电机运行标志位


void Follow_Route(void);
void Control(void);





#endif