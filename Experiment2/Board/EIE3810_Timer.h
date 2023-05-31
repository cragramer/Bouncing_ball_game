#ifndef __EIE3810_Timer_H
#define __EIE3810_Timer_H
#include "stm32f10x.h"
void EIE3810_TIM3_PWMInit(u16 arr, u16 psc);
void EIE3810_TIM3_Init(u16 arr, u16 psc);
void TIM3_IRQHandler(void);
void EIE3810_TIM4_Init(u16 arr, u16 psc);
void TIM4_IRQHandler(void);
void EIE3810_SYSTICK_Init();

#endif