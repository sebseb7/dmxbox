
#include "menu_directdmx.h"
#include "menu_directdmx_help.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;


void menu_directdmx_help()
{


	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);
		
		uint16_t text_width =  get_text_width_16pt("Direct DMX Help");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Direct DMX Help", 200,200,255);



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
				set_current_execution(menu_directdmx);
			}
		}
		
		if(field == 9)
		{
		}
	}
}

