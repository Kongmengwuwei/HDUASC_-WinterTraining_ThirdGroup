#ifndef __ENCODER_H
#define __ENCODER_H

#include "zf_common_headfile.h"

//×óÂÖ±àÂëÆ÷
#define ENCODER_1                   (QTIMER1_ENCODER1)
#define ENCODER_1_A                 (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_1_B                 (QTIMER1_ENCODER1_CH2_C1)

//ÓÒÂÖ±àÂëÆ÷
#define ENCODER_2                   (QTIMER1_ENCODER2)
#define ENCODER_2_A                 (QTIMER1_ENCODER2_CH1_C2)
#define ENCODER_2_B                 (QTIMER1_ENCODER2_CH2_C24)

extern float leftcount,rightcount;
extern float LeftSpeed, RightSpeed, Last_LeftSpeed, Last_RightSpeed;
extern float AveSpeed, DifSpeed;

void Encoder_Init(void);
void Read_Encoder(void);
void Speed_Get(void);

#endif
