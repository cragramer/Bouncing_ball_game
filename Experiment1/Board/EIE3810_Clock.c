#include "stm32f10x.h"
#include "EIE3810_Clock.h"

void EIE3810_clock_tree_init(void)
{
	u8 PLL = 7;
	u8 temp = 0;
	RCC->CR |= 0x00010000; //Enable HSE clock, HSE oscillator is ON
	while (!((RCC->CR >> 17) & 0x1));//Examine whether external high speed clock (HSE oscillator) is ready
	RCC->CFGR &= 0xFFFDFFFF;
	RCC->CFGR |= 1 << 16; //HSE oscillator clock selected as PLL input clock
	RCC->CFGR |= PLL << 18; //PLL input clock x 9
	RCC->CR |= 0x01000000;//PLL enabled, PLL is ON
	while (!(RCC->CR >> 25));//while PLL is unlocked
	RCC->CFGR &= 0xFFFFFFFE;
	RCC->CFGR |= 0x00000002;//PLL selected as system clock
	while (temp != 0x02) //examine whether PLL is selected
	{
		temp = RCC->CFGR >> 2;
		temp &= 0x03; //Check the value of temp
	}
	RCC->CFGR &= 0xFFFFFC0F;
	RCC->CFGR |= 0x00000400;//HCLK  divided  by 2	
	//RCC->CFGR &= 0xFFFFC7FF; //HCLK  not divided  by 2	
	FLASH->ACR = 0x32;// 	set Two wait states, if 48 MHz < SYSCLK ≤ 72 MHz, Half cycle is disabled, Prefetch is enabled, Prefetch buffer is disabled
	RCC->APB1ENR |= 1 << 17; //USART2 clock enabled
	RCC->APB2ENR |= 1 << 14;  //USART1 clock enabled
}
