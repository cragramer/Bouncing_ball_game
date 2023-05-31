#ifndef __EIE3810_Joypad_H
#define __EIE3810_Joypad_H
#include "stm32f10x.h"

void JOYPAD_Init(void);
void JOYPAD_Delay(u16 t);
u8 JOYPAD_Read(void);

#endif
