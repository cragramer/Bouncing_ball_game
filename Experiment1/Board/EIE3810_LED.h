#ifndef __EIE3810_LED_H
#define __EIE3810_LED_H
#include "stm32f10x.h"

// put procudure header here



void EIE3810_LED_Init(void);
void LED0_on(void);
void LED0_off(void);
int  LED0_read(void);
void toggle_led1(void);
int LED1_on(void);
int LED1_off(void);






#endif
