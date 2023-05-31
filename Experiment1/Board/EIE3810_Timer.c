
#include "stm32f10x.h"
#include "EIE3810_Clock.h"
#include "EIE3810_Timer.h"
#include "EIE3810_LED.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_Joypad.h"
#define LED0_PWM_VAL TIM3->CCR2
u8 task1HeartBeat, task2HeartBeat, task3HeartBeat, task4HeartBeat;
void EIE3810_TIM3_PWMInit(u16 arr, u16 psc)
{
	RCC->APB2ENR |= 1 << 3; //IOPB enable
	GPIOB->CRL &= 0xFF0FFFFF;
	GPIOB->CRL |= 0x00B00000; //port5, output mode with push and pull
	RCC->APB2ENR |= 1 << 0; //AFIO enable
	AFIO->MAPR &= 0xFFFFF3FF;//bit 11,10=0
	AFIO->MAPR |= 1 << 11; //TIM3 partial remap
	RCC->APB1ENR |= 1 << 1; //TIM3 enable
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->CCMR1 |= 7 << 12; //output compare 2 mode
	TIM3->CCMR1 |= 1 << 11; //output compare 2 preload enable
	TIM3->CCER |= 1 << 4; //capture/compare 2 output enable
	TIM3->CR1 = 0x0080; //auto-reload preload enable, TIM3_ARR register is buffered
	TIM3->CR1 |= 1 << 0; //counter enable
}

void EIE3810_TIM3_Init(u16 arr, u16 psc)
{
	//TIM3
	RCC->APB1ENR |= 1 << 1; //TIM3 enable
	TIM3->ARR = arr; //auto reload register
	TIM3->PSC = psc; //pre-scaler
	TIM3->DIER |= 1 << 0; //update interrupt enabled
	TIM3->CR1 |= 0x01; //counter enabled
	NVIC->IP[29] = 0x45;  //priority register of TIM3
	NVIC->ISER[0] = (1 << 29);//enable TIM3
}

void TIM3_IRQHandler(void) {
	vu8 temp = JOYPAD_Read();
	//EIE3810_TFTLCD_FillRectangle(0, 48, 0, 16, WHITE);
	if (((temp >> 0) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE); //reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 33, BLACK, WHITE); //'A'
	}
	else if (((temp >> 1) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 34, BLACK, WHITE);//'B'
	}
	else if (((temp >> 2) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 51, BLACK, WHITE);//'S'
		EIE3810_TFTLCD_ShowChar(8, 0, 37, BLACK, WHITE);//'E'
		EIE3810_TFTLCD_ShowChar(16, 0, 44, BLACK, WHITE); //'L'
		EIE3810_TFTLCD_ShowChar(24, 0, 37, BLACK, WHITE);//'E'
		EIE3810_TFTLCD_ShowChar(32, 0, 35, BLACK, WHITE);//'C'
		EIE3810_TFTLCD_ShowChar(40, 0, 52, BLACK, WHITE);//'T'
	}
	else if (((temp >> 3) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 51, BLACK, WHITE);//'S'
		EIE3810_TFTLCD_ShowChar(8, 0, 52, BLACK, WHITE);//'T'
		EIE3810_TFTLCD_ShowChar(16, 0, 33, BLACK, WHITE);//'A'
		EIE3810_TFTLCD_ShowChar(24, 0, 50, BLACK, WHITE);//'R'
		EIE3810_TFTLCD_ShowChar(32, 0, 52, BLACK, WHITE);//'T'
	}
	else if (((temp >> 4) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 53, BLACK, WHITE);//'U'
		EIE3810_TFTLCD_ShowChar(8, 0, 48, BLACK, WHITE);//'P'
	}
	else if (((temp >> 5) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 36, BLACK, WHITE);//'D'
		EIE3810_TFTLCD_ShowChar(8, 0, 47, BLACK, WHITE);//'O'
		EIE3810_TFTLCD_ShowChar(16, 0, 55, BLACK, WHITE);//'W'
		EIE3810_TFTLCD_ShowChar(24, 0, 46, BLACK, WHITE);//'N'
	}
	else if (((temp >> 6) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 44, BLACK, WHITE); //'L'
		EIE3810_TFTLCD_ShowChar(8, 0, 37, BLACK, WHITE);//'E'
		EIE3810_TFTLCD_ShowChar(16, 0, 38, BLACK, WHITE); //'F'
		EIE3810_TFTLCD_ShowChar(24, 0, 52, BLACK, WHITE);//'T'
	}
	else if (((temp >> 7) & 0x01) == 1) {
		EIE3810_TFTLCD_clean(WHITE);//reflash the screen
		EIE3810_TFTLCD_ShowChar(0, 0, 50, BLACK, WHITE);//'R'
		EIE3810_TFTLCD_ShowChar(8, 0, 41, BLACK, WHITE);//'I'
		EIE3810_TFTLCD_ShowChar(16, 0, 39, BLACK, WHITE);//'G'
		EIE3810_TFTLCD_ShowChar(24, 0, 40, BLACK, WHITE);//'H'
		EIE3810_TFTLCD_ShowChar(32, 0, 52, BLACK, WHITE);//'T'
	}
}


void EIE3810_TIM4_Init(u16 arr, u16 psc)
{
	//TIM3
	RCC->APB1ENR |= 1 << 2; //TIM4 enable
	TIM4->ARR = arr; //auto reload register
	TIM4->PSC = psc; //pre-scaler
	TIM4->DIER |= 1 << 0; //update interrupt enabled
	TIM4->CR1 |= 0x01; //counter enabled
	NVIC->IP[30] = 0x45;  //priority register of TIM3
	NVIC->ISER[0] = (1 << 30);//enable TIM3
}

void TIM4_IRQHandler(void)
{
	if (TIM4->SR & 1 << 0) //update interrupt flag
	{
		GPIOE->ODR ^= 1 << 5; //port5 output data
	}
	TIM4->SR &= ~(1 << 0); //clear the flag
}


void EIE3810_SYSTICK_Init()
{
	//SYSCLK
	SysTick->CTRL = 0; //CLear SysTick->CTRL setting
	SysTick->LOAD = 89999;
	SysTick->CTRL = 3;
	//CLKSOURCE=0: FLCK/8
	//CLKSOURCE=1: FLCK
	//CLKSOURCE=0 is synchronized and better than CLKSOURCE=1
	//Refer to clock tree on page 92 of RM0008
}


