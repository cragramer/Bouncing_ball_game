#ifndef __EIE3810_CORE_H
#define __EIE3810_CORE_H
#include "stm32f10x.h"
#include "EIE3810_LED.h"
#include "EIE3810_Clock.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_USART.h"
#include "EIE3810_Key.h"
#include "EIE3810_TFTLCD.h"
void EIE3810_TFTLCD_ShowString(u16 x, u16 y, u8 big, char* string, u16 color, u16 bgcolor);
void EIE3810_TFTLCD_Welcome(void);
int EIE3810_next_page(void);
void EIE3810_third_page(void);
void set_number(void);
void set_direction(void);
void direction_lookup(u8 DIRECTION);
void move_the_ball(int speedx, int speedy);
int  hit_the_upper(void);
int hit_the_lower(void);
int hit_the_left(void);
int hit_the_right(void);
int hit_board_1(void);
int hit_board_2(void);
void move_board_1(u8 speed_board_1,u8 dir);
void move_board_2(u8 speed_board_2,u8 dir);
void draw_the_ball(u16 color);
void draw_the_board_1(u16 color);
void draw_the_board_2(u16 color);
void display_time(u16 x, u16 y);
void check_pause(void);
void print_end_screen(void);
void move_board(void);
void show_bounce(void);
void USART1_IRQHandler(void);
#endif
