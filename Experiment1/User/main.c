#include "stm32f10x.h"
#include "EIE3810_Clock.h"
#include "EIE3810_LED.h"
#include "EIE3810_USART.h"
#include "EIE3810_TFTLCD.h"
#include "stm32f10x_it.h"
#include "EIE3810_Core.h"
#include "EIE3810_key.h"
#include "EIE3810_Joypad.h"
#include "EIE3810_Buzzer.h"
#include "EIE3810_Timer.h"
/*-------------------- */
/*----------------------- Variable--------------------------*/
int mode;
u32  DIRECTION = 100;
extern u16  board_1_x, board_1_y, board_2_x, board_2_y;
extern u16  ball_x, ball_y;
extern int ball_x_speed, ball_y_speed;
extern u16  board_1_wide, board_2_wide;
extern u8  board_1_speed, board_2_speed, ball_radius;
extern u8 game_end, game_start;
extern u8 task1HeartBeat, task2HeartBeat, task3HeartBeat, task4HeartBeat;
extern u8 ball_period;
extern u8 board_period;
extern u16 time;
extern u8 board_1_high, board_2_high;
extern u8 game_pause;
extern u8 can_pause;
/*-------------------- */
/*----------------------- Function--------------------------*/
void Delay(u32);
void EIE3810_NVIC_SetPriorityGroup(u8 prigroup);

void Delay(u32 count)
{
	u32 i;
	for (i = 0;i < count;i++);
}

void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp1, temp2;
	temp2 = prigroup & 0x00000007;
	temp2 <<= 8; // Correspond to bits [10:8] in AIRCR
	temp1 = SCB->AIRCR; // Read value of AIRCR
	temp1 &= 0x0000F8FF;// Reset bits [10:8] and bits [31:16]
	temp1 |= 0x05FA0000; // Set register key as 0x05FA
	temp1 |= temp2;
	SCB->AIRCR = temp1;
}

int main(void) {
	ball_x=240;
	///ball_y=800-board_1_y-board_1_high-ball_radius;
	ball_y=770;
	EIE3810_clock_tree_init();
	EIE3810_NVIC_SetPriorityGroup(5);
	JOYPAD_Init();
	//EIE3810_TIM3_Init(99, 7199);
	EIE3810_SYSTICK_Init();
	EIE3810_TFTLCD_Init();//initialize the screen
	EIE3810_TFTLCD_clean(WHITE);//set the screen white
	EIE3810_KEY_Init();
	EIE3810_LED_Init();
	EIE3810_Buzzer_Init();
	EIE3810_TFTLCD_Welcome(); //show the welcome screen
	Delay(0x1FFFFF);//set a delay
	mode = EIE3810_next_page();//print the next page
	if (mode==1){ball_period=ball_period/2;}
	EIE3810_third_page();//print the third page
	set_direction();
	Delay(0x7FFFFF);//set a delay
	set_number();
	game_start = 1;
	game_pause = 0;
	EIE3810_TFTLCD_clean(WHITE);
	draw_the_board_1(BLACK);
	draw_the_board_2(BLACK);
	display_time(300, 100);
	show_bounce();
	while (game_end == 0) {
		if (game_pause == 0) {
			//move the board
			if (task2HeartBeat >= board_period) {
				task2HeartBeat = 1;
				move_board();
			}
			//move the ball
			if (task1HeartBeat >= ball_period) {
				task1HeartBeat = 1;
				move_the_ball(ball_x_speed, ball_y_speed);
			}
			//show the time and the bounce
			if (task3HeartBeat >= 100) {
				task3HeartBeat = 1;
				time++;
				display_time(300, 100);
				show_bounce();
				can_pause = 1;
			}
		}
	}
	game_end = 1; //game end
	print_end_screen();
	while (1)
	{
		;
	}
}


