#include "stm32f10x.h"
#include "EIE3810_Clock.h"
#include "EIE3810_LED.h"
#include "EIE3810_USART.h"
#include "EIE3810_TFTLCD.h"
#include "stm32f10x_it.h"
#include "EIE3810_Core.h"
#include "EIE3810_key.h"
void JOYPAD_Init(void)
{
	RCC->APB2ENR |= 1 << 3; //IOPB enable
	RCC->APB2ENR |= 1 << 5; //IOPD enable
	GPIOB->CRH &= 0XFFFF00FF; //together with the next line 
	GPIOB->CRH |= 0X00003800; //output push-pull 10; input 9
	GPIOB->ODR |= 3 << 10; //output=11 10,9 
	GPIOD->CRL &= 0XFFFF0FFF; //together with the next line 
	GPIOD->CRL |= 0X00003000; //output push-pull 3
	GPIOD->ODR |= 1 << 3; //output=11 3
}

void JOYPAD_Delay(u16 t)
{
	while (t--);
}

u8 JOYPAD_Read(void)
{
	vu8 temp = 0;
	u8 t;
	// The processor sends a pulse to JOYPAD to make it put the key info to PB10
	GPIOB->BSRR |= 1 << 11; // set PB11
	Delay(80); // Delay the time required by PB11 time sequence protocol to activate JOYPAD
	GPIOB->BSRR |= 1 << 27; // reset PB11
	for (t = 0;t < 8;t++)
	{
		temp >>= 1;
		// shift the last key record forward to prepare for recording current key at the current MSB
		// after shifting for 8 loops, each key record is in the position whose index corresponds to this key
		if ((((GPIOB->IDR) >> 10) & 0x01) == 0) temp |= 0x80;// read PB10: if it is low, mark temp's current MSB to record key
		GPIOD->BSRR |= (1 << 3); // set PD3
		Delay(80);// Keep PD3 high for the time required by PD3 time sequence protocol of key info reading
		GPIOD->BSRR |= (1 << 19); // reset PD3
		Delay(80);// Keep PD3 low for the time required by PD3 time sequence protocol of key info reading
	}
	return temp;
}
