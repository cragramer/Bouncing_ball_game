#include "stm32f10x.h"
#include "EIE3810_LED.h"
#include "EIE3810_Clock.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_USART.h"
#include "EIE3810_Key.h"
#include "EIE3810_Core.h"
#include "EIE3810_KEY.h"
#include "EIE3810_Joypad.h"
#include "EIE3810_Buzzer.h"
#include "EIE3810_Timer.h"

/*----------------------- Variable--------------------------*/
u32 extern DIRECTION;
u16  board_1_x = 240, board_1_y = 790, board_2_x = 240, board_2_y = 10;
u16  ball_x = 100, ball_y = 100;
int ball_x_speed, ball_y_speed;
u8  can_pause = 1;
u8  board_1_speed = 10, board_2_speed = 10;
u16 board_1_wide = 100, board_2_wide = 100;
u8  board_1_high = 20, board_2_high = 20;
u8 ball_radius = 10;
u8 game_end = 0;
u8 game_pause = 0;
u8 game_time = 0;
u8 number_of_bounce = 0;
u16 time;
u8 ball_period = 5;
u8 board_period = 10;
u8 game_start = 0;
u8 win = 2;
u8 eat = 0;
u8 press = 0;

/*----------------------- Function--------------------------*/
//a function to show string
void EIE3810_TFTLCD_ShowString(u16 x, u16 y, u8 big, char* string, u16 color, u16 bgcolor) {
	if (big == 0) {
		while (*string != '\0') {
			EIE3810_TFTLCD_ShowChar(x, y, *string, color, bgcolor);
			x += 8;
			string++;
		}
	}
	else if (big == 1) {
		while (*string != '\0') {
			EIE3810_TFTLCD_ShowChar2412(x, y, *string, color, bgcolor);
			x += 12;
			string++;
		}
	}
}

//a function to show welcome screen
void EIE3810_TFTLCD_Welcome(void) {
	//print welcome to mini project!
	EIE3810_TFTLCD_ShowString(90, 120, 1, "Welcome to mini Project!", WHITE, BLUE);
	//print this is the final project
	//set a delay
	Delay(0x3FFFFF);
	EIE3810_TFTLCD_ShowString(110, 240, 0, "This is the Final Lab.", WHITE, RED);
	//print Are you ready
	//set a delay
	Delay(0x3FFFFF);
	EIE3810_TFTLCD_ShowString(110, 280, 0, "Are you ready?", WHITE, RED);
	//set a delay
	Delay(0x3FFFFF);
	//print OK! let's get start.
	EIE3810_TFTLCD_ShowString(110, 320, 0, "OK! Let's start.", WHITE, RED);
}

//a function to show the first page
int EIE3810_next_page(void) {
	//print please select the difficulty level
	//clear the screen
	u8 mode;
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_ShowString(120, 120, 0, "Please select the difficulty level", WHITE, RED);

	//press KEY0 to enter 
	EIE3810_TFTLCD_ShowString(120, 240, 0, "Press KEY0 to enter", WHITE, RED);
	//print Easy
	EIE3810_TFTLCD_ShowString(120, 160, 0, "Easy", WHITE, BLUE);
	//print hard
	EIE3810_TFTLCD_ShowString(120, 180, 0, "Hard", BLUE, WHITE);
	mode = 0; //0 - easy; 1 - hard;
	while (1) {
		//LED0_on();
		if (key_0_on() == 1) {
			break;
		}
		if (key_up_on() == 1) {//easy mode 
			//LED0_on();
			EIE3810_TFTLCD_ShowString(120, 160, 0, "Easy", WHITE, BLUE);
			//print hard
			EIE3810_TFTLCD_ShowString(120, 180, 0, "Hard", BLUE, WHITE);
		}
		if (key_1_on() == 1) { //hard mode
			mode = 1;
			//print Easy
			EIE3810_TFTLCD_ShowString(120, 160, 0, "Easy", BLUE, WHITE);
			//print hard
			EIE3810_TFTLCD_ShowString(120, 180, 0, "Hard", WHITE, BLUE);
		}
	}
	return mode;
}

//a function to show the third page
void EIE3810_third_page(void) {
	//print Use USART for a random direction
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_ShowString(120, 160, 0, "Use USART for a random direction", WHITE, RED);
	//EIE3810_USART1_init(72, 400000);
}


// a function to count number
void set_number(void) {
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_SevenSegment(203, 470, 3, BLUE);
	Delay(10000000);
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_SevenSegment(203, 470, 2, BLUE);
	Delay(10000000);
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_SevenSegment(203, 470, 1, BLUE);
	Delay(10000000);
	EIE3810_TFTLCD_clean(WHITE);
	EIE3810_TFTLCD_SevenSegment(203, 470, 0, BLUE);
	Delay(10000000);
	EIE3810_TFTLCD_clean(WHITE);
}

//set the direction of the ball
void set_direction(void) {
	/* recieve the direction signal from USART1 */
	//EIE3810_TFTLCD_DrawBackground(WHITE);
	EIE3810_USART1_init(72, 19000); // set unique baud rate
	while (DIRECTION == 100) // RXNE: read data register not empty
	{
		USART1_IRQHandler();
	}
	//print the direction	
	EIE3810_TFTLCD_clean(WHITE);
	//LED1_off();
	EIE3810_TFTLCD_ShowString(200, 400, 0, "The direction is: ", WHITE, BLACK);
	//print direction
	EIE3810_TFTLCD_ShowChar(200 + 8 * 18, 400, 48 + DIRECTION, WHITE, BLACK);
	Delay(0x3FFFFF);
	direction_lookup(DIRECTION);
}

//a fuction to look up the direction 
void direction_lookup(u8 DIRECTION) {
	//set the direction of the ball
	if (DIRECTION == 0) {
		ball_x_speed = 4;
		ball_y_speed = 1;
	}
	if (DIRECTION == 1) {
		ball_x_speed = 3;
		ball_y_speed = 2;
	}
	if (DIRECTION == 2) {
		ball_x_speed = 2;
		ball_y_speed = 3;
	}
	if (DIRECTION == 3) {
		ball_x_speed = 1;
		ball_y_speed = 4;
	}
	if (DIRECTION == 4) {
		ball_x_speed = -1;
		ball_y_speed = 4;
	}
	if (DIRECTION == 5) {
		ball_x_speed = -2;
		ball_y_speed = 3;
	}
	if (DIRECTION == 6) {
		ball_x_speed = -3;
		ball_y_speed = 2;
	}
	if (DIRECTION == 7) {
		ball_x_speed = -4;
		ball_y_speed = 1;
	}
	ball_y_speed = -ball_y_speed;
}

//move the ball one step 
void move_the_ball(int speedx, int speedy) {
	extern u16 ball_x, ball_y;
	u16 ball_x_old, ball_y_old;
	ball_x_old = ball_x;
	ball_y_old = ball_y;
	if (game_end == 1) { return; }
	eat = 0;
	//judge if the ball hit the board 1
	if (hit_board_1()) {
		ball_y_speed = -ball_y_speed;
		eat = 1;
	}
	//judge if the ball hit the board 2
	if (hit_board_2()) {
		ball_y_speed = -ball_y_speed;
		eat = 1;
	}
	//judge if the ball hit the left
	if (hit_the_left()) {
		ball_x_speed = -ball_x_speed;
	}
	//judge if the ball hit the right
	if (hit_the_right()) {
		ball_x_speed = -ball_x_speed;
	}
	//judge if the ball hit the upper
	if (hit_the_upper()==1 && hit_board_2()==0) {
		game_end = 1;
		win = 1;
		ball_x_speed = 0;
		ball_y_speed = 0;
	}
	//judge if the ball hit the lower
	if (hit_the_lower()==1 && hit_board_1()==0) {
		game_end = 1;
		win = 0;
		ball_x_speed = 0;
		ball_y_speed = 0;
	}
	//draw_the_old_ball
	EIE3810_TFTLCD_DrawCircle(ball_x_old, ball_y_old, ball_radius, 1, WHITE);
	//move the ball
	ball_x = ball_x + ball_x_speed;
	ball_y = ball_y + ball_y_speed;
	//draw the ball
	draw_the_ball(BLACK);
	draw_the_board_1(BLACK);
	draw_the_board_2(BLACK);
}

int hit_the_upper() {
	if (ball_y + ball_y_speed <= ball_radius) {
		win = 1;
		return 1;
	}
	else {
		return 0;
	}
}

int hit_the_lower() {
	if (ball_y + ball_y_speed >= 800 - ball_radius) {
		win = 0;
		return 1;
	}
	else {
		return 0;
	}
}

int hit_the_left() {
	if (ball_x + ball_x_speed <= ball_radius) {
		makebuzzer();
		return 1;
	}
	else {
		return 0;
	}
}

int hit_the_right() {
	if (ball_x + ball_x_speed >= 480 - ball_radius) {
		makebuzzer();
		return 1;
	}
	else {
		return 0;
	}
}

int hit_board_1() {
	if (ball_y_speed < 0) { return 0; }
	if (ball_x + ball_x_speed >= board_1_x - board_1_wide / 2 &&
		ball_x + ball_x_speed <= board_1_x + board_1_wide / 2 &&
		ball_y + ball_y_speed >= board_1_y - board_1_high / 2 - ball_radius)
	{
		number_of_bounce++;
		show_bounce();
		makebuzzer();
		return 1;
	}
	else {
		return 0;
	}
}

int hit_board_2() {
	if (ball_y_speed > 0) { return 0; }
	if (ball_x + ball_x_speed >= board_2_x - board_2_wide / 2 &&
		ball_x + ball_x_speed <= board_2_x + board_2_wide / 2 &&
		ball_y + ball_y_speed <= board_2_y + board_2_high / 2 + ball_radius)
	{
		number_of_bounce++;
		show_bounce();
		makebuzzer();
		return 1;
	}
	else {
		return 0;
	}
}

//mover board 1
void move_board_1(u8 speed_board_1, u8 dir) {
	// draw the board
	if (key_2_on() == 1 && dir == 1 && board_1_x >= board_1_wide / 2) {
		draw_the_board_1(WHITE);
		board_1_x -= speed_board_1;
		//LED0_on();
		draw_the_board_1(BLACK);
	}
	if (key_0_on() == 1 && dir == 0 && board_1_x <= 480 - board_1_wide / 2) {
		draw_the_board_1(WHITE);
		board_1_x += speed_board_1;
		draw_the_board_1(BLACK);

	}
}

//mover board 2
void move_board_2(u8 speed_board_2, u8 dir) {
	// draw the board
	if (dir == 1 && board_2_x >= board_2_wide / 2) {
		draw_the_board_2(WHITE);
		board_2_x -= speed_board_2;
	}
	if (dir == 0 && board_2_x <= 480 - board_2_wide / 2) {
		draw_the_board_2(WHITE);
		board_2_x += speed_board_2;
	}
	//draw the board
	draw_the_board_2(BLACK);
}

void draw_the_ball(u16 color) {
	EIE3810_TFTLCD_DrawCircle(ball_x, ball_y, ball_radius, 1, color);
}

//draw the board 1
void draw_the_board_1(u16 color) {
if (board_1_x - board_1_wide / 2 > 0 && board_1_x + board_1_wide / 2 < 480) {
		EIE3810_TFTLCD_FillRectangle(board_1_x - board_1_wide / 2, board_1_wide, board_1_y - board_1_high / 2, board_1_high, color);
	}
	if (board_1_x - board_1_wide / 2 < 0) {
		EIE3810_TFTLCD_FillRectangle(0, board_1_wide, board_1_y - board_1_high / 2, board_1_high, color);
	}
	if (board_1_x + board_1_wide / 2 > 480) {
		EIE3810_TFTLCD_FillRectangle(480 - board_1_wide, board_1_wide, board_1_y - board_1_high / 2, board_1_high, color);
	}
}

//draw the board 2
void draw_the_board_2(u16 color) {
	if (board_2_x - board_2_wide / 2 > 0 && board_2_x + board_2_wide / 2 < 480) {
		EIE3810_TFTLCD_FillRectangle(board_2_x - board_2_wide / 2, board_2_wide, board_2_y - board_2_high / 2, board_2_high, color);
	}
	if (board_2_x - board_2_wide / 2 < 0) {
		EIE3810_TFTLCD_FillRectangle(0, board_2_wide, board_2_y - board_2_high / 2, board_2_high, color);
	}
	if (board_2_x + board_2_wide / 2 > 480) {
		EIE3810_TFTLCD_FillRectangle(480 - board_2_wide, board_2_wide, board_2_y - board_2_high / 2, board_2_high, color);
	}
}

//check if the game is paused or not 
void check_pause() {
	if (!press && (key_1_on() == 1 || ((JOYPAD_Read() >> 3) & 0x01) == 1) && game_pause == 0) {
		game_pause = 1;
		press = 1;
	}
	else if (!press && (key_1_on() == 1 || ((JOYPAD_Read() >> 3) & 0x01) == 1) && game_pause == 1) {
		game_pause = 0;
		press = 1;
	}
	if (press && key_1_on() == 0 && ((JOYPAD_Read() >> 3) & 0x01) == 0) press = 0;
}

//show the time
void display_time(u16 x, u16 y) {
	u32 minute, second;
	u8 min1, min0, sec1, sec0;
	minute = time / 60;
	second = time % 60;
	min1 = minute / 10;
	min0 = minute % 10;
	sec1 = second / 10;
	sec0 = second % 10;
	EIE3810_TFTLCD_ShowChar(x, y, 48 + min1, BLUE, WHITE);
	EIE3810_TFTLCD_ShowChar(x + 8, y, 48 + min0, BLUE, WHITE);
	EIE3810_TFTLCD_ShowString(x + 16, y, 0, "min", BLUE, WHITE);
	EIE3810_TFTLCD_ShowChar(x + 40, y, 48 + sec1, BLUE, WHITE);
	EIE3810_TFTLCD_ShowChar(x + 48, y, 48 + sec0, BLUE, WHITE);
	EIE3810_TFTLCD_ShowString(x + 56, y, 0, "s", BLUE, WHITE);
}

//show the bounce number
void show_bounce() {
	u8 bounce1, bounce0;
	bounce1 = number_of_bounce / 10;
	bounce0 = number_of_bounce % 10;
	EIE3810_TFTLCD_ShowString(400 - 8 * 7-8*5, 80, 0, "Bounce:", BLUE, WHITE);
	EIE3810_TFTLCD_ShowChar(400-8*5, 80, 48 + bounce1, BLUE, WHITE);
	EIE3810_TFTLCD_ShowChar(408-8*5, 80, 48 + bounce0, BLUE, WHITE);
}

//print the end screen
void print_end_screen(void) {
	if (game_end == 0) { return; }
	EIE3810_TFTLCD_clean(WHITE);
	Delay(10000000);
	EIE3810_TFTLCD_ShowString(120, 388, 1, "Player ", BLUE, YELLOW);
	if (win == 1) EIE3810_TFTLCD_ShowChar2412(192, 388, 'A', BLUE, YELLOW);
	else EIE3810_TFTLCD_ShowChar2412(192, 388, 'B', BLUE, YELLOW);
	EIE3810_TFTLCD_ShowString(204, 388, 1, " wins", BLUE, YELLOW);
	Delay(50000000);
}

//move the board 
void move_board() {
	if (key_2_on() == 1) { move_board_1(board_1_speed, 1); }
	if (key_0_on() == 1) { move_board_1(board_1_speed, 0); }
	if (JOYPAD_Read() == 1 << 6) { move_board_2(board_2_speed, 1); }
	if (JOYPAD_Read() == 1 << 7) { move_board_2(board_2_speed, 0); }
}

void USART1_IRQHandler(void){
	if (USART1->SR & (1<<5) ) // RXNE: read data register not empty
	{
		DIRECTION = USART1->DR; // the data recieved by UASRT1
		//DIRECTION=2;
		//LED0_on();
	}
	
}
