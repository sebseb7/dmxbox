#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#include "choose_device_class.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"
#include "dmxbox_hal.h"
#include "dmx_device_class.h"

static void (*current_execution)(void);
static void choose_device_class(void);

#define MAXLENGTH 30

static uint8_t redraw = 1;
static uint8_t active_row = 0;
static uint8_t scroll_offset = 0;

void invoke_choose_device_class(uint32_t uuid)
{
	redraw=1;

	active_row=0;
	scroll_offset=0;
	for(int i = 0; i < get_device_class_count();i++)
	{
		if(get_device_class(i)->uuid == uuid)
		{
			active_row=i+1;
			if(active_row >= 8)
			{
				scroll_offset = active_row-7;
			}
		}
	}
	
	current_execution=get_current_execution();
	set_current_execution(choose_device_class);
}

uint32_t get_choose_device_class(void)
{
	return get_device_class(active_row-1)->uuid;
}


static void draw_background(uint8_t drawrow,uint8_t row)
{
	if(row == active_row)
	{
		draw_filledRect(53,53+(drawrow*18),127,14,155,100,100);
	}
	else
	{
		draw_filledRect(53,53+(drawrow*18),127,14,60,60,60);
	}
}

static uint8_t current_lineitem=0;

static void init_lineitems(void)
{
	current_lineitem=0;
}
static void draw_lineitem(char* value)
{
	if((current_lineitem < scroll_offset+8)&&(current_lineitem>=scroll_offset))
	{
		draw_background(current_lineitem-scroll_offset,current_lineitem);
		draw_text_8x6(57,53+(current_lineitem-scroll_offset)*18,value,255,0,255);
	}
	current_lineitem++;
}
static void draw_scrollbar(void)
{
	if(current_lineitem>8)
	{
		draw_filledRect(190,53,10,140,50,50,50);
		float perline = 140.0/(float)current_lineitem;
		draw_filledRect(190,53+(scroll_offset*perline),10,perline*8.0,50,150,50);

	}
}

void choose_device_class()
{
	if(redraw)
	{
		clear_buttons();
		
		int x, y;
		if(redraw==1)
		{
			for(x = 0; x < LCD_WIDTH; x++) {
				for(y = 0; y < LCD_HEIGHT; y++) {
				
					uint8_t red;
					uint8_t green;
					uint8_t blue;
				
					getLedXY(x,y,&red,&green,&blue);
					uint16_t brightness = red+green+blue;
					red = brightness/4;
					green = brightness/4;
					blue = brightness/4;
					setLedXY(x,y,red,green,blue);
				}
			}
		}
		redraw=0;
		
		draw_filledRect(35,35,LCD_WIDTH-70,LCD_HEIGHT-70,255,255,255);
		draw_filledRect(40,40,LCD_WIDTH-80,LCD_HEIGHT-80,55,55,55);
		
		draw_button_h(220,53,52,42,"^",155,0,0,0,255,0);
		draw_button_h(220,100,52,42,"OK",155,0,0,0,255,0);
		draw_button_h(220,147,52,42,"v",155,0,0,0,255,0);

		init_lineitems();
		char buf2[30];
		draw_lineitem("<empty>");
		for(int i = 0; i < get_device_class_count();i++)
		{
			snprintf(buf2,30,"%s",get_device_class(i)->name);
			draw_lineitem(buf2);
		}
		draw_scrollbar();



	}
	

	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		if(y > 144)
		{
			if(active_row < (get_device_class_count()))
			{
				active_row++;
				int drawline = active_row-scroll_offset; 
				if(drawline==8)
				{
					scroll_offset++;
				}
			}
			redraw=2;
		}
		else if(y > 98)
		{
			redraw=1;
			set_current_execution(current_execution);
		}
		else
		{
			if(active_row > 0)
			{
				if(active_row==scroll_offset)
				{
					scroll_offset--;
				}

				active_row--;
			}
			redraw=2;
		}
	}
}


