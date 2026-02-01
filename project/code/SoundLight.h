#ifndef __SOUNDLIGHT_H
#define __SOUNDLIGHT_H

#include "zf_common_headfile.h"

#define LED				(B9)
#define Buzzer			(B11)
#define Act_Duration	(5)

extern uint8 LEDstat, Buzzerstat;

void SoundLight_Init(void);
void SoundLight_React(void);

#endif
