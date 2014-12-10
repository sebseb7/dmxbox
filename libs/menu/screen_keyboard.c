#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#include "screen_keyboard.h"
#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;
static uint8_t redraw_text = 1;

void screen_keyboard(void);
void screen_keyboard_numeric(void);

static const uint8_t keyboard_layout[11*4] = {
											'1','2','3','4','5','6','7','8','9','0',' ',
											'Q','W','E','R','T','Z','U','I','O','P',' ',
											' ','A','S','D','F','G','H','J','K','L',' ',
											' ','Y','X','C','V','B','N','M',127,126,' '
											};

static uint8_t cursor_pos = 0;
static uint8_t blink = 0;

#define MAXLENGTH 30

static char buffer[MAXLENGTH+1];
static uint8_t buffer_length=0;

static void (*current_execution)(void);

static char* current_title;

void invoke_keyboard(char* desc, char* initial)
{
	redraw=1;
	redraw_text=1;
	snprintf(buffer,30,"%s",initial);
	buffer_length=strlen(buffer);
	cursor_pos=buffer_length;
	current_execution=get_current_execution();
	set_current_execution(screen_keyboard);
	current_title=desc;
}

void invoke_numeric_keyboard(char* desc, uint32_t initial)
{
	redraw=1;
	redraw_text=1;
	snprintf(buffer,30,"%"PRIu32"",(unsigned int)initial);
	buffer_length=strlen(buffer);
	cursor_pos=buffer_length;
	current_execution=get_current_execution();
	set_current_execution(screen_keyboard_numeric);
	current_title=desc;
}

char* get_keyboard_buffer()
{
	return buffer;
}

uint32_t get_keyboard_number()
{
	return strtol(buffer, NULL, 10);
}

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

		uint16_t text_width =  get_text_width_16pt(current_title);

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, current_title, 200,200,255);

		
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
	
	if(redraw_text)
	{
		draw_filledRect(67,51,MAXLENGTH*6+9,11,100,0,0);
		draw_text_8x6(70,50,buffer,255,100,100);
		redraw_text=0;
	}

	if((blink>>3) % 2== 0)
	{
		draw_text_8x6(70+cursor_pos*6,51,"_",100,0,0);
	}
	else
	{
		draw_text_8x6(70+cursor_pos*6,51,"_",255,255,255);
	}
	blink++;



	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		printf("%i %i\n",x,y);

		char keychar = 0;

		if(y > 203)
		{
			keychar=' ';

		}else if(y < 41)
		{
			if(x < 40)
			{
				redraw=1;
				set_current_execution(current_execution);
				buffer[0]=0;
			}

		}else if(y > 83 ){


			uint8_t keypos = 0;

			if(y > 176)
			{
				keypos+=33;
			}else if(y > 150)
			{
				keypos+=22;
			}else if(y > 122)
			{
				keypos+=11;
			}


			if(keypos==0)
			{
				if(x > 280) keychar = 127; 
				if(x > 253) keypos++;
				if(x > 226) keypos++;
				if(x > 199) keypos++;
				if(x > 172) keypos++;
				if(x > 145) keypos++;
				if(x > 117) keypos++;
				if(x > 91) keypos++;
				if(x > 64) keypos++;
				if(x > 36) keypos++;
			}
			else if(keypos==11)
			{
				if(x > 292) keychar = 13; 
				if(x > 265) keypos++;
				if(x > 238) keypos++;
				if(x > 211) keypos++;
				if(x > 184) keypos++;
				if(x > 157) keypos++;
				if(x > 130) keypos++;
				if(x > 103) keypos++;
				if(x > 76) keypos++;
				if(x > 49) keypos++;
			}
			else if(keypos==22)
			{
				if(x > 280) keychar = 13; 
				if(x > 253) keypos++;
				if(x > 226) keypos++;
				if(x > 199) keypos++;
				if(x > 172) keypos++;
				if(x > 145) keypos++;
				if(x > 117) keypos++;
				if(x > 91) keypos++;
				if(x > 64) keypos++;
				keypos++;
			}
			else if(keypos==33)
			{
				if(x > 238) keychar=1;
				if(x > 265) keychar++;
				if(x > 211) keypos++;
				if(x > 184) keypos++;
				if(x > 157) keypos++;
				if(x > 130) keypos++;
				if(x > 103) keypos++;
				if(x > 76) keypos++;
				keypos++;
			}
			if(keychar == 0)
				keychar = keyboard_layout[keypos];
		}

		if(keychar == 0)
		{
		}else if(keychar == 1)
		{
			if(cursor_pos>0) 
			{
				cursor_pos--;
				redraw_text=1;
			}
		}else if(keychar == 2)
		{
			if(cursor_pos<buffer_length) 
			{
				cursor_pos++;
				redraw_text=1;
			}
		}else if(keychar == 13)
		{
			set_current_execution(current_execution);
		}else if(keychar == 127)
		{
			if((cursor_pos > 0) && (buffer_length > 0))
			{
				for(int i = cursor_pos;i <=  cursor_pos+(buffer_length-cursor_pos);i++)
				{	
					buffer[i-1]=buffer[i];
				}
				cursor_pos--;
				buffer_length--;
				redraw_text=1;
			}
		}else{
			if((cursor_pos < MAXLENGTH) && (buffer_length < MAXLENGTH))
			{
				for(int i = cursor_pos+(buffer_length-cursor_pos);i>=cursor_pos;i--)
				{
					buffer[i+1]=buffer[i];
				}
				
				buffer[cursor_pos]=keychar;
				cursor_pos++;
				buffer_length++;
				redraw_text=1;
			}
		}

		printf("%i %c\n",keychar,keychar);




	}
}


void screen_keyboard_numeric()
{

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt(current_title);

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, current_title, 200,200,255);

		
		for(int row=0;row<4;row++)
		{
			for(int chan=0;chan<11;chan++)
			{//298*

				if((row==0)&&(chan>9)) continue;
				if(row==1) continue;
				if(row==2) continue;
				if((row==3)&&(chan>9)) continue;
				if((row==3)&&(chan<8)) continue;
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

		draw_text_8x6(11+10+(10*(25+2))-6,6+50+45+(0*(25+2)),"DEL",255,255,255);
		draw_text_8x6(11+10+(10*(25+2))-6,6+50+45+(2*(25+2)),"RET",255,255,255);
	}
	
	if(redraw_text)
	{
		draw_filledRect(67,51,MAXLENGTH*6+9,11,100,0,0);
		draw_text_8x6(70,50,buffer,255,100,100);
		redraw_text=0;
	}

	if((blink>>3) % 2== 0)
	{
		draw_text_8x6(70+cursor_pos*6,51,"_",100,0,0);
	}
	else
	{
		draw_text_8x6(70+cursor_pos*6,51,"_",255,255,255);
	}
	blink++;



	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		printf("%i %i\n",x,y);

		char keychar = 0;

		if(y > 203)
		{
			keychar=' ';

		}else if(y < 41)
		{
			if(x < 40)
			{
				redraw=1;
				set_current_execution(current_execution);
				buffer[0]=0;
			}

		}else if(y > 83 ){


			uint8_t keypos = 0;

			if(y > 176)
			{
				keypos+=33;
			}else if(y > 122)
			{
				if(x > 280) keychar = 13; 
			}
			else
			{


				if(keypos==0)
				{
					if(x > 280) keychar = 127; 
					if(x > 253) keypos++;
					if(x > 226) keypos++;
					if(x > 199) keypos++;
					if(x > 172) keypos++;
					if(x > 145) keypos++;
					if(x > 117) keypos++;
					if(x > 91) keypos++;
					if(x > 64) keypos++;
					if(x > 36) keypos++;
				}
			}

			if(keypos==33)
			{
				if(x > 238) keychar=1;
				if(x > 265) keychar++;
				if(x > 211) keypos++;
				if(x > 184) keypos++;
				if(x > 157) keypos++;
				if(x > 130) keypos++;
				if(x > 103) keypos++;
				if(x > 76) keypos++;
				keypos++;
			}
			if(keychar == 0)
				keychar = keyboard_layout[keypos];
		}

		if(keychar == 0)
		{
		}else if(keychar == 1)
		{
			if(cursor_pos>0) 
			{
				cursor_pos--;
				redraw_text=1;
			}
		}else if(keychar == 2)
		{
			if(cursor_pos<buffer_length) 
			{
				cursor_pos++;
				redraw_text=1;
			}
		}else if(keychar == 13)
		{
			set_current_execution(current_execution);
		}else if(keychar == 127)
		{
			if((cursor_pos > 0) && (buffer_length > 0))
			{
				for(int i = cursor_pos;i <=  cursor_pos+(buffer_length-cursor_pos);i++)
				{	
					buffer[i-1]=buffer[i];
				}
				cursor_pos--;
				buffer_length--;
				redraw_text=1;
			}
		}else{
			if((cursor_pos < MAXLENGTH) && (buffer_length < MAXLENGTH))
			{
				for(int i = cursor_pos+(buffer_length-cursor_pos);i>=cursor_pos;i--)
				{
					buffer[i+1]=buffer[i];
				}

				buffer[cursor_pos]=keychar;
				cursor_pos++;
				buffer_length++;
				redraw_text=1;
			}
		}

		printf("%i %c\n",keychar,keychar);




	}
}


