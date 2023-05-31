#include "stm32f10x.h"
#include "EIE3810_USART.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_LED.h"
u32 extern DIRECTION; 
//u8 extern DIRECTION_SETTED;

void EIE3810_USART2_init(u32 pclk1, u32 baudrate)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	//write the formula 
	temp = (float)(pclk1 * 1000000) / (baudrate * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4; // move mantissa for operation USART2->BRR
	mantissa += fraction;
	RCC->APB2ENR |= 1 << 2; //IOPA(IO portA clock) enabled	
	GPIOA->CRL &= 0xFFFF00FF;
	GPIOA->CRL |= 0x00008B00; //CNF3 input with pull up/down, CNF2 output function push-pull, max speed 50 HZ
	RCC->APB1RSTR |= 1 << 17; //USARAT2 reset
	RCC->APB1RSTR &= ~(1 << 17); //USARAT2 back
	USART2->BRR = mantissa; //Set matissa of USART2 divider
	USART2->CR1 = 0x2008; //USART2 transmission enable
}


void EIE3810_USART1_init(u32 pclk2, u32 baudrate)
{
	//USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk2 * 1000000) / (baudrate * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= 1 << 2;
	GPIOA->CRH &= 0xFFFFF00F;
	GPIOA->CRH |= 0x000008B0; //CNF2 input with pull up/down, CNF1 output function push-pull, max speed 50 HZ
	RCC->APB2RSTR |= 1 << 14; //USARAT1 reset
	RCC->APB2RSTR &= ~(1 << 14);
	USART1->BRR = mantissa;
	USART1->CR1 = 0x2008; //USART1 transmission enable
	USART1->CR1 |= 1 << 2;
	USART1->CR1 |= 1 << 5;
	USART1->CR1 |= 0x24;
	//LED0_on();
}



void USART_print(u8 USARTport, char* st)
{
	u8 i = 0;
	while (st[i] != 0x00)
	{
		//select the transmission port USARTport1
		if (USARTport == 1) {
			USART1->DR = st[i]; //send the input message
			//check the TXE register bit
			while (!(USART1->SR >> 7));
			if (i == 255) break;
			i++;
		}
		//select the transmission port USARTport2
		if (USARTport == 2) {
			USART2->DR = st[i]; //send the input message
			//check the TXE register bit
			while (!(USART2->SR >> 7));
			if (i == 255) break;
			i++;
		}
	}
}

void EIE3810_USART1_EXTIInit(void) {
	NVIC->IP[37] = 0X65;
	NVIC->ISER[1] |= 1 << 5;
}



