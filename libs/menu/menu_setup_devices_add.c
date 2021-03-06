
#include "menu_setup_devices.h"
#include "screen_keyboard.h"
#include "choose_device_class.h"
#include "menu_setup_devices_add.h"
#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_device_class.h"
#include "dmx_device.h"

#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static uint8_t redraw = 1;

static uint8_t init_device=1;

static dmx_device_t *new_device;

static uint8_t invoke=0;

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
static void draw_lineitem_addr(char* name,uint16_t num_value)
{
	if((current_lineitem < scroll_offset+10)&&(current_lineitem>=scroll_offset))
	{
		char value[30];
		snprintf(value,sizeof(value),"%i",num_value);

		draw_background(current_lineitem-scroll_offset,current_lineitem);
		draw_text_8x6(10,45+(current_lineitem-scroll_offset)*18,name,255,255,0);
		draw_text_8x6(80,45+(current_lineitem-scroll_offset)*18,value,255,0,255);
		for(int i = 0; i < 9;i++)
		{
			draw_filledRect(140+(i*7),45+2+(current_lineitem-scroll_offset)*18,6,10,100,100,200);
			
			if(((1<<i)&num_value)==(1<<i))
			{
				draw_filledRect(141+(i*7),45+3+(current_lineitem-scroll_offset)*18,4,4,255,255,255);
			}
			else
			{
				draw_filledRect(141+(i*7),45+7+(current_lineitem-scroll_offset)*18,4,4,255,255,255);
			}
			
		}
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

void menu_setup_devices_add()
{
	if(init_device)
	{
		new_device=my_malloc(sizeof(dmx_device_t));
		init_device=0;
		new_device->name=my_malloc(1);
		new_device->name[0]=0;
		new_device->uuid=rand();
		new_device->device_class_uuid=0;
		new_device->base_address=0;
		
		active_row = 0;
		scroll_offset = 0;

	}
	
	if(invoke != 0)
	{
		if(invoke==1)
		{
			char* name = get_keyboard_buffer();
			my_free(new_device->name);
			new_device->name=my_malloc(strlen(name)+1);
			strcpy(new_device->name, name);
		}
		else if(invoke==2)
		{
			new_device->device_class_uuid = get_choose_device_class();
		}
		else if(invoke==3)
		{
			new_device->base_address = get_keyboard_number();
		}
		invoke=0;
	}

	if(redraw)
	{
		clearDisplay();
		clear_buttons();
		redraw=0;

		draw_filledRect(0,0,LCD_WIDTH,35,155,100,100);

		draw_filledCircle(17,17,15,40,80,40);
		draw_text_8x6(7,10,"Back",255,100,100);

		uint16_t text_width =  get_text_width_16pt("Add Device");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Add Device", 200,200,255);

		init_lineitems();
		draw_lineitem("Name",new_device->name);
		if(new_device->device_class_uuid == 0)
		{
			draw_lineitem("Class","<empty>");
		}
		else if(get_device_class_by_uuid(new_device->device_class_uuid) == NULL)
		{
			draw_lineitem("Class","<missing>");
		}
		else
		{
			draw_lineitem("Class",get_device_class_by_uuid(new_device->device_class_uuid)->name);
		}
		draw_lineitem_addr("Address",new_device->base_address);


		draw_scrollbar();

		draw_button_h(257,45,52,42,"^",155,0,0,0,255,0);
		draw_button_h(257,92,52,42,"Edit",155,0,0,0,255,0);
		draw_button_h(257,139,52,42,"Save",155,0,0,0,255,0);
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
				init_device=1;
				my_free(new_device->name);
				my_free(new_device);
				set_current_execution(menu_setup_devices);
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
			if(active_row < 2)
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
			invoke=active_row+1;
			redraw=1;
			if(active_row==0)
			{
				invoke_keyboard("Enter Device Name",new_device->name);
			}
			else if(active_row==1)
			{
				invoke_choose_device_class(new_device->device_class_uuid);
			}
			else if(active_row==2)
			{
				invoke_numeric_keyboard("Enter Base Address",new_device->base_address);
			}
		}
		if(field == 3)
		{
			add_device(new_device);
			redraw=1;
			init_device=1;
			set_current_execution(menu_setup_devices);

		}
	}
}

