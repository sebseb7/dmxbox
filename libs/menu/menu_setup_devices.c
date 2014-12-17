
#include "menu_setup.h"
#include "menu_setup_devices.h"
#include "menu_setup_devices_add.h"
#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_device.h"
#include "my_malloc.h"

#include <stdio.h>

static uint8_t redraw = 1;

static uint8_t active_row = 0;
static uint8_t scroll_offset = 0;

static void draw_background(uint8_t drawrow,uint8_t row)
{
	if(row == active_row)
	{
		draw_filledRect(6,45+(drawrow*18),200,14,155,100,100);
	}
	else
	{
		draw_filledRect(6,45+(drawrow*18),200,14,60,60,60);
	}
}

static uint8_t current_lineitem=0;

static void init_lineitems(void)
{
	current_lineitem=0;
}
static void draw_lineitem(char* name,char* value)
{
	if((current_lineitem < scroll_offset+10)&&(current_lineitem>=scroll_offset))
	{
		draw_background(current_lineitem-scroll_offset,current_lineitem);
		draw_text_8x6(10,45+(current_lineitem-scroll_offset)*18,name,255,255,0);
		draw_text_8x6(80,45+(current_lineitem-scroll_offset)*18,value,255,0,255);
	}
	current_lineitem++;
}
static void draw_scrollbar(void)
{
	if(current_lineitem>10)
	{
		draw_filledRect(230,45,10,176,50,50,50);
		float perline = 176.0/(float)current_lineitem;
		draw_filledRect(230,45+(scroll_offset*perline),10,perline*10.0,50,150,50);

	}
}

void menu_setup_devices()
{

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("Setup Devices");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Setup Devices", 200,200,255);


		init_lineitems();
		char buf[30];
		char buf2[30];
		draw_lineitem("","<new>");
		for(int i = 0; i < get_device_count();i++)
		{
			snprintf(buf,30,"%i",i+1);
			snprintf(buf2,30,"%s",get_device(i)->name);
			draw_lineitem(buf,buf2);
		}

		draw_scrollbar();

		draw_button_h(257,45,52,42,"^",155,0,0,0,255,0);
		if((get_device_count()<10)&&(active_row==0))
		{
			draw_button_h(257,92,52,42,"Add",155,0,0,0,255,0);
		}
		if(active_row>0)
		{
			draw_button_h(257,92,52,42,"Edit",155,0,0,0,255,0);
			draw_button_h(257,139,52,42,"Delete",155,0,0,0,255,0);
		}
		draw_button_h(257,186,52,42,"v",155,0,0,0,255,0);
	}

	uint8_t field=0;
	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		if(y > 41)
		{
			field=1;

			if(y > 184)
			{
				field=4;
			}else if(y > 136)
			{
				field=3;
			}else if(y > 89)
			{
				field=2;
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
		
		if(field == 1)
		{
			if(active_row > 0)
			{
				if(active_row==scroll_offset)
				{
					scroll_offset--;
				}

				active_row--;
			}
			redraw=1;
		}
		if(field == 4)
		{
			if(active_row < (get_device_count()))
			{
				active_row++;
				int drawline = active_row-scroll_offset; 
				if(drawline==10)
				{
					scroll_offset++;
				}
			}
			redraw=1;
		}
		if(field == 2)
		{
			redraw=1;
			if(active_row==0)
			{
				if(get_device_count()<10)
				{
					set_current_execution(menu_setup_devices_add);
				}
			}
			else
			{
				//set_current_execution(menu_setup_device_edit);
				//set_device_to_edit(active_row-1);
			}
		}
		if(field == 3)
		{
			redraw=1;
			
			if(active_row > 0)
			{
				del_device(active_row-1);
				if(active_row==scroll_offset)
				{
					scroll_offset--;
				}
				active_row--;
			}

		}
	}
}

