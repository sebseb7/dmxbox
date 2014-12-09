
#include "menu_setup_device_classes.h"
#include "screen_keyboard.h"
#include "menu_setup_device_classes_add.h"
#include "main.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"

#include "dmx_device_class.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
struct {

	uint8_t name_length;
	char* name;
	uint8_t channels;
	uint8_t *channel_defaults; 
	uint16_t *channel_defaults_blackout; // will be called when blackout is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_identify; // will be called when identify is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_fullbright; // will be called when fullbright is issued , 0xffff means , do not overwrite
	char **channel_names;

} device_class_t;
*/

static uint8_t redraw = 1;

static uint8_t init_device=1;

static dmx_device_class_t new_device_class;

static uint8_t active_row = 0;
static uint8_t scroll_offset = 0;

static uint8_t invoke=0;


void draw_background(uint8_t row)
{
	if(row == active_row)
	{
		draw_filledRect(6,45+(row*30),200,14,155,100,100);
	}
	else
	{
		draw_filledRect(6,45+(row*30),200,14,60,60,60);
	}
}

void menu_setup_device_classes_add()
{
	if(init_device)
	{
		init_device=0;
		new_device_class.name="";
		new_device_class.name_alloc=0;
		new_device_class.channels=1;
		active_row = 0;
		scroll_offset = 0;

	}

	if(invoke != 0)
	{
		if(invoke==1)
		{
			printf("nameip\n");
			char* name = get_keyboard_buffer();
			if(new_device_class.name_alloc)
				free(new_device_class.name);
			new_device_class.name=malloc(strlen(name)+1);
			new_device_class.name_alloc=1;
			strcpy(new_device_class.name, name);
		}
		if(invoke==2)
		{
			uint8_t ch = get_keyboard_number();
			if((ch > 0) &&(ch < 16))
			{
				new_device_class.channels = ch;
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

		uint16_t text_width =  get_text_width_16pt("Add Device Model");

		draw_text_16pt((LCD_WIDTH-text_width)>>1,9, "Add Device Model", 200,200,255);

		
		draw_background(0);
		draw_text_8x6(10,45,"Name",255,255,0);
		draw_text_8x6(80,45,new_device_class.name,255,0,255);
		draw_background(1);
		draw_text_8x6(10,75,"Channels",255,255,0);
		draw_number_8x6(80,75,new_device_class.channels,2,'0',255,0,255);

		for(int i = 0;i < new_device_class.channels;i++)
		{
			char strbuf[30];
			snprintf(strbuf,sizeof(strbuf),"Ch. %i Name",i+1);
			draw_background(2+i*3);
			draw_text_8x6(10,105+(i*30),strbuf,255,255,0);
			draw_background(2+i*3+1);
			draw_background(2+i*3+2);
		}


		draw_button_icon(257,45,52,1,"^",155,0,0,0,255,0);
		draw_button_icon(257,110,52,1,"Edit",155,0,0,0,255,0);
		draw_button_icon(257,175,52,1,"v",155,0,0,0,255,0);
		


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
				init_device=1;
				if(new_device_class.name_alloc)
					free(new_device_class.name);
				set_current_execution(menu_setup_device_classes);
			}
		}
		
		if(field == 3)
		{
			if(active_row > 0) active_row--;
			redraw=1;
		}
		if(field == 9)
		{
			if(active_row < 10) active_row++;
			redraw=1;
		}
		if(field == 6)
		{
			if(active_row==0)
			{
				invoke_keyboard("Enter Class Name",new_device_class.name);
				invoke=1;
				redraw=1;
			}
			if(active_row==1)
			{
				invoke_numeric_keyboard("Number of Channels",new_device_class.channels);
				invoke=2;
				redraw=1;
			}
		}
	}
}

