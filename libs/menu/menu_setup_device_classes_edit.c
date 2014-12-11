
#include "menu_setup_device_classes.h"
#include "screen_keyboard.h"
#include "menu_setup_device_classes_edit.h"
#include "dmxbox_hal.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_device_class.h"

#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
typedef struct {

	uint8_t name_length;
	char* name;
	uint8_t channels;
	uint8_t channels_allocated;
	uint8_t *channel_defaults; 
	int16_t *channel_defaults_blackout; // will be called when blackout is issued , -1 means , do not overwrite
	int16_t *channel_defaults_identify; // will be called when identify is issued , -1 means , do not overwrite
	int16_t *channel_defaults_fullbright; // will be called when fullbright is issued , -1 means , do not overwrite
	char **channel_names;

} dmx_device_class_t;
*/

static uint8_t redraw = 1;

static uint8_t init_device=1;

static dmx_device_class_t *new_device_class;


static uint8_t invoke=0;

#define PRE_ALLOCATE 8

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

static uint8_t device_to_edit=0;
void set_device_class_to_edit(uint8_t idx)
{
	device_to_edit=idx;

}

void menu_setup_device_classes_edit()
{
	if(init_device)
	{
		new_device_class=get_device_class(device_to_edit);
		init_device=0;
		
		active_row = 0;
		scroll_offset = 0;

	}

	if(invoke != 0)
	{
		if(invoke==1)
		{
			//printf("nameip\n");
			char* name = get_keyboard_buffer();
			my_free(new_device_class->name);
			new_device_class->name=my_malloc(strlen(name)+1);
			strcpy(new_device_class->name, name);
		}
		else if(invoke==2)
		{
			uint8_t ch = get_keyboard_number();
			if((ch > 0) &&(ch <= PRE_ALLOCATE))
			{
				new_device_class->channels = ch;
			}
		}
		else if(invoke>2)
		{
			if(invoke%2==1)
			{
				char* name = get_keyboard_buffer();
				my_free(new_device_class->channel_names[(invoke-2)>>1]);
				new_device_class->channel_names[(invoke-2)>>1]=my_malloc(strlen(name)+1);
				strcpy(new_device_class->channel_names[(invoke-2)>>1], name);
			}
			else
			{
				uint8_t def = get_keyboard_number();
				new_device_class->channel_defaults[(invoke-3)>>1] = def;

			}
			
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

		uint16_t text_width =  get_text_width_16pt("Edit Device Model");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Edit Device Model", 200,200,255);

		init_lineitems();
		char buf[30];
		draw_lineitem("Name",new_device_class->name);
		snprintf(buf,30,"%i",new_device_class->channels);
		draw_lineitem("Channels",buf);

		for(int i = 0;i < new_device_class->channels;i++)
		{
			char strbuf[30];
			snprintf(strbuf,sizeof(strbuf),"Ch. %i Name",i+1);
			draw_lineitem(strbuf,new_device_class->channel_names[i]);
			snprintf(strbuf,sizeof(strbuf),"%i",new_device_class->channel_defaults[i]);
			draw_lineitem("Default",strbuf);
		}

		draw_scrollbar();

		draw_button_h(257,45,52,42,"^",155,0,0,0,255,0);
		draw_button_h(257,92,52,42,"Edit",155,0,0,0,255,0);
		
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
				set_current_execution(menu_setup_device_classes);
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
			if(active_row < (new_device_class->channels*2+2)-1)
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
				invoke_keyboard("Enter Class Name",new_device_class->name);
			}
			else if(active_row==1)
			{
				invoke_numeric_keyboard("Number of Channels",new_device_class->channels);
			}
			else if(active_row>1)
			{
				if(active_row%2==0)
				{
					char strbuf[30];
					snprintf(strbuf,sizeof(strbuf),"Ch. %i Name",active_row>>1);
					//printf("inv: %s %i \n",strbuf,(active_row-2)>>1);
					invoke_keyboard(strbuf,new_device_class->channel_names[(active_row-2)>>1]);
				}else{
					char strbuf[30];
					snprintf(strbuf,sizeof(strbuf),"Ch. %i Default",(active_row-1)>>1);
					//printf("inv: %s %i \n",strbuf,(active_row-3)>>1);
					invoke_numeric_keyboard(strbuf,new_device_class->channel_defaults[(active_row-3)>>1]);
				}
			}
		}
	}
}

