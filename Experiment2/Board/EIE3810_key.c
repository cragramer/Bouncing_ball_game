#include "stm32f10x.h"
#include "EIE3810_KEY.h"

// put your procedure and code here

void EIE3810_KEY_Init(void)
{
  //enable the RCC clock of Port E
  RCC->APB2ENR |= 1 << 6;
  GPIOE->CRL &= 0xFFFFF0FF;
  GPIOE->CRL |= 0x00000800;
  GPIOE->ODR = 1 << 2;//set the PortE ODR5 bit

  //enable the RCC clock of Port E
  RCC->APB2ENR |= 1 << 6;
  GPIOE->CRL &= 0xFFFF0FFF;
  GPIOE->CRL |= 0x00008000;
  GPIOE->ODR |= 1 << 3; //set PortE ODR3 output data as 1

  //enable the RCC clock of PortA
  RCC->APB2ENR |= 1 << 2;
  //set Key_Up(PA0) as input mode
  GPIOA->CRL &= 0xFFFFFFF0;
  GPIOA->CRL |= 0x00000008;
  GPIOA->BRR |= 0x00000001;//set PortA ODR0 output data as 0

  RCC->APB2ENR |= 1 << 6;
  GPIOE->CRL &= 0xFFF0FFFF;
  GPIOE->CRL |= 0x00080000;
  GPIOE->ODR |= 1 << 4; //set PortE ODR3 output data as 1

}

int key_0_on(void) {
  if ((GPIOE->IDR & 1 << 4) == 0) { //check the output data of PE3 
    return 1;
  }
  else { return 0; }
}

int key_1_on(void) {
  if ((GPIOE->IDR & 0x00000008) == 0x00000000) { //check the output data of PE3 
    return 1;
  }
  else { return 0; }
}

int key_2_on(void) {
  if ((GPIOE->IDR & 0x00000004) == 0x00000000) { //check the output data of PE2
    return 1;
  }
  else { return 0; }
}

int key_up_on(void) {
  if ((GPIOA->IDR & 0x00000001) == 0x00000001) { //check the output data of PA0
    return 1;
  }
  else { return 0; }
}