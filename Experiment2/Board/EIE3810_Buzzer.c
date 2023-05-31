#include "stm32f10x.h"
#include "EIE3810_Buzzer.h"

// put your procedure and code here

void EIE3810_Buzzer_Init(void)
{
	RCC->APB2ENR |= 1 << 3;
	GPIOB->CRH &= 0xFFFFFFF0;
	GPIOB->CRH |= 0x00000003;
}

u8 Buzzer_Toggle(u8 toggle)
{
	if (toggle == 0)
	{
		GPIOB->BSRR = 1 << 8; // on
		return 1;
	}
	else
	{
		GPIOB->BRR = 1 << 8; // off
		return 0;
	}
}

void makebuzzer() {
	GPIOB->BSRR = 1 << 8; // on()
	//set a delay
	Delay(0x1FFFF);
	GPIOB->BRR = 1 << 8;
}