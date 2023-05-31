#include "stm32f10x.h"
#include "EIE3810_LED.h"
extern 
// put your code here
void EIE3810_LED_Init(void)
{
	//enable the RCC clock of PortB
	RCC->APB2ENR |= 1 << 3;
	//set LED0(PB5) as general purpose output push-pull mode, with max speed 50 Hz
	GPIOB->CRL &= 0xFF0FFFFF;
	GPIOB->CRL |= 0x00300000;
	GPIOB->BSRR |= 1 << 5;	//set the PortE ODR5 bit

	//enable the RCC clock of PortE
	RCC->APB2ENR |= 1 << 6;
	//set LED1(PE5) as general purpose output push-pull mode, with max speed 50 Hz
	GPIOE->CRL &= 0xFF0FFFFF;
	GPIOE->CRL |= 0x00300000;
	GPIOE->BSRR |= 1 << 5;		//set the PortE ODR5 bit
}

void LED0_off(void) { //turn off the LED0
	GPIOB->BSRR |= 1 << 5;
}

void LED0_on(void) {  //turn on the LED0
	GPIOB->BRR |= 1 << 5;
}

int  LED0_read(void) {
	if (((GPIOB->ODR >> 5) & 1) == 0) { //read the output data of LED0
		return 1;
	}
	else { return 0; }
}

int LED1_off(void) {
	GPIOE->BSRR |= 1 << 5; //turn off the LED1
}

int LED1_on(void) {
	GPIOE->BRR |= 1 << 5; //turn on the LED1
}

int LED1_read(void) {
	if (((GPIOE->ODR >> 5) & 1) == 0) {//read the output data of LED1
		return 1;
	}
	else { return 0; }
}

void toggle_led1(void)
{
	if (LED1_read() == 0) { //if LED0 is off, turn on the LED1
		LED1_on();
	}
	else {
		LED1_off(); //otherwise turn off the LED0
	}
}