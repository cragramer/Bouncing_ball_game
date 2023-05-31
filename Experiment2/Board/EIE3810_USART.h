#ifndef EIE3810_USART_H
#define EIE3810_USART_H

void EIE3810_USART1_init(u32 pclk2, u32 baudrate);
void EIE3810_USART2_init(u32 pclk1, u32 baudrate);
void USART_print(u8 USARTport, char* st);
void EIE3810_USART1_EXTIInit(void);


#endif
