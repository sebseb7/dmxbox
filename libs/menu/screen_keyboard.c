#include <stdio.h>

#include "menu_main.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;


static uint8_t keyboard_layout[11*4] = {
											'1','2','3','4','5','6','7','8','9','0',' ',
											  'Q','W','E','R','T','Z','U','I','O','P',' ',
											' ','A','S','D','F','G','H','J','K','L',' ',
											  ' ','Y','X','C','V','B','N','M',127,126,' '
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
			for(int chan=0;chan<11;chan++)
			{//298*

				if((row==0)&&(chan>9)) continue;
				if((row==1)&&(chan>9)) continue;
				if((row==2)&&(chan>9)) continue;
				if((row==2)&&(chan<1)) continue;
				if((row==3)&&(chan>9)) continue;
				if((row==3)&&(chan<1)) continue;
				uint8_t offset=0;
				if(row%2) offset=12;
				if(keyboard_layout[row*11+chan] > 125)
				{
					//offset+=15;
					draw_filledRect(10+(chan*(25+2))+offset,50+45+(row*(25+2)),25,25,110,110,80);
				}else{
					draw_filledRect(10+(chan*(25+2))+offset,50+45+(row*(25+2)),25,25,100,100,100);
				}
	
				char buf[2];
				sprintf(buf, "%c", keyboard_layout[row*11+chan]);

				draw_text_8x6(11+10+(chan*(25+2))+offset,6+50+45+(row*(25+2)),buf,255,255,255);

			}
		}
		draw_filledRect(10+(10*(25+2))+0,50+45+(0*(25+2)),25,25,110,90,90);//backspace
		draw_filledRect(10+(10*(25+2))+12,50+45+(1*(25+2)),25-12,25+2,80,80,120);//enter part1
		draw_filledRect(10+(10*(25+2))+0,50+45+(2*(25+2)),25,25,80,80,120);//enter part2
		draw_filledRect(10+( 2*(25+2))+0,50+45+(4*(25+2)),25*7+2*6,25,80,120,80);//space

		draw_text_8x6(11+10+(10*(25+2))-6,6+50+45+(0*(25+2)),"DEL",255,255,255);
		draw_text_8x6(11+10+(10*(25+2))-6,6+50+45+(2*(25+2)),"RET",255,255,255);
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

