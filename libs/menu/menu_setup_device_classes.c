
#include "menu_setup.h"
#include "menu_setup_device_classes.h"
#include "menu_setup_device_classes_add.h"
#include "menu_callback.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

static uint8_t redraw = 1;

void menu_setup_device_classes()
{

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("Setup Device Models");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Setup Device Models", 200,200,255);


		draw_button_icon(217,45,92,1,"^",155,0,0,0,255,0);
		draw_button_icon(217,110,92,1,"Add",155,0,0,0,255,0);
		draw_button_icon(217,175,92,1,"v",155,0,0,0,255,0);
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
				set_current_execution(menu_setup);
			}
		}
		
		if(field == 6)
		{
			redraw=1;
			set_current_execution(menu_setup_device_classes_add);
		}
	}
}

