#include <stdio.h>

#include "menu_main.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;


static uint8_t keyboard_layout[12*4] = {
											'1','2','3','4','5','6','7','8','9','0',' ',' ',
											  'Q','W','E','R','T','Z','U','I','O','P',' ',' ',
											' ','A','S','D','F','G','H','J','K','L',' ',' ',
											  ' ','Y','X','C','V','B','N','M',' ',' ',' ',' '
											};

static uint8_t cursor_pos = 0;
static uint8_t blink = 0;


void screen_keyboard()
{

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("enter blah");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "enter blah", 200,200,255);

		
		for(int row=0;row<4;row++)
		{
			for(int chan=0;chan<12;chan++)
			{//298*

				if((row==0)&&(chan>9)) continue;
				if((row==1)&&(chan>9)) continue;
				if((row==2)&&(chan>9)) continue;
				if((row==2)&&(chan<1)) continue;
				if((row==3)&&(chan>7)) continue;
				if((row==3)&&(chan<1)) continue;
				uint8_t offset=0;
				if(row%2) offset=12;
				draw_filledRect(11+(chan*(23+2))+offset,60+45+(row*(23+2)),23,23,100,100,100);
	
				char buf[2];
				sprintf(buf, "%c", keyboard_layout[row*12+chan]);

				draw_text_8x6(10+11+(chan*(23+2))+offset,5+60+45+(row*(23+2)),buf,255,255,255);

			}
		}
		draw_filledRect(11+(10*(23+2))+0,60+45+(0*(23+2)),23*2+2*1,23,100,100,100);//backspace
		draw_filledRect(11+(10*(23+2))+12,60+45+(1*(23+2)),23*2+2*1-12,23+2,100,100,100);//enter part1
		draw_filledRect(11+(10*(23+2))+0,60+45+(2*(23+2)),23*2+2*1,23,100,100,100);//enter part2
		draw_filledRect(11+(0*(23+2))+0,60+45+(4*(23+2)),23*12+2*10,23,100,100,100);//space

	}
		
	draw_text_8x6(70,50,"123456789012345678901234567890",255,100,100);

	if((blink>>3) % 2== 0)
	{
		draw_text_8x6(20,20,"_",0,0,0);
	}
	else
	{
		draw_text_8x6(20,20,"_",255,255,255);
	}
	blink++;



	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		if(y > 41)
		{
		}
		else
		{
			if(x < 40)
			{
				redraw=1;
				set_current_execution(menu_main);
			}
		}
		
	}
}

