
#include "menu_directdmx.h"
#include "menu_directdmx_help.h"
#include "menu_main.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;


static uint8_t direct_ch[8*3];

static uint8_t initialized = 0;

void menu_directdmx()
{

	if(! initialized)
	{
		initialized=1;
		for(int i = 0 ; i < 8*3;i++)
			direct_ch[i]=0;
			
	}

	for(int i=0;i<8;i++)
	{
		if(MIDI_get_fader_updated(i))
		{
			direct_ch[i]=MIDI_get_fader(i);
			redraw=1;
		}
	}

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		clear_buttons_midi();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);
		
		draw_filledCircle(320-17,17,15,40,80,40);
		draw_text_8x6(320-27,10,"Help",255,100,100);

		uint16_t text_width =  get_text_width_16pt("Direct DMX");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Direct DMX", 200,200,255);

		for(int row=0;row<3;row++)
		{
			for(int chan=0;chan<8;chan++)
			{
				draw_filledRect(11+(chan*(33+5)),45+(row*(58+5)),33,58,100,100,200);

				uint8_t barsize = 54 - (direct_ch[chan] * 0.211764f);

				draw_filledRect(11+(chan*(33+5))+2,45+(row*(58+5))+2+barsize,33-4,58-(4+barsize),100,200,100);
			}
		}


	}

	uint8_t field=0;
	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		if(y > 41)
		{
			field+=1;

			if(y > 171)
			{
				field+=6;
			}else if(y > 105)
			{
				field+=3;
			}

			if(x > 212)
			{
				field+=2;
			}else if (x > 109)
			{
				field+=1;
			}
		}
		else
		{
			if(x < 40)
			{
				redraw=1;
				set_current_execution(menu_main);
			}
			if(x > 280)
			{
				redraw=1;
				set_current_execution(menu_directdmx_help);
			}
		}
		
		if(field == 9)
		{
		}
	}
}

