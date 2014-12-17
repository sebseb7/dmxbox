#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


#include "info_popup.h"
#include "mcugui/rect.h"
#include "mcugui/text.h"
#include "mcugui/circle.h"
#include "mcugui/button.h"
#include "dmxbox_hal.h"

static void (*current_execution)(void);
static void info_popup(void);

#define MAXLENGTH 30

static char popup_text1[MAXLENGTH+1];
static char popup_text2[MAXLENGTH+1];
static char popup_text3[MAXLENGTH+1];

static uint8_t redraw = 1;
static uint8_t type = 0;
static uint8_t response = 0;

void invoke_info_popup(char* desc1,char* desc2,char* desc3)
{
	redraw=1;
	type=1;
	strncpy(popup_text1,desc1,MAXLENGTH);
	strncpy(popup_text2,desc2,MAXLENGTH);
	strncpy(popup_text3,desc3,MAXLENGTH);
	current_execution=get_current_execution();
	set_current_execution(info_popup);
}
void invoke_yes_no_popup(char* desc1,char* desc2,char* desc3)
{
	redraw=1;
	type=2;
	strncpy(popup_text1,desc1,MAXLENGTH);
	strncpy(popup_text2,desc2,MAXLENGTH);
	strncpy(popup_text3,desc3,MAXLENGTH);
	current_execution=get_current_execution();
	set_current_execution(info_popup);
}
uint8_t get_yes_no_response(void)
{
	return response;
}

void info_popup()
{
	if(redraw)
	{
		clear_buttons();
		redraw=0;
		
		int x, y;
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
		
		draw_filledRect(50,50,LCD_WIDTH-100,LCD_HEIGHT-100,255,255,255);
		draw_filledRect(55,55,LCD_WIDTH-110,LCD_HEIGHT-110,55,55,55);


		if(type==1)
		{
			draw_button_h(120,150, 80,30,"OK",155,0,0,0,255,0);
		}
		else
		{
			draw_button_h(100,150, 80,30,"Yes",0,155,0,255,0,0);
			draw_button_h(140,150, 80,30,"No",155,0,0,0,255,0);
		}
	


		draw_text_8x6((LCD_WIDTH>>1)-(strlen(popup_text1)*3),70,popup_text1,255,100,100);
		draw_text_8x6((LCD_WIDTH>>1)-(strlen(popup_text2)*3),90,popup_text2,255,100,100);
		draw_text_8x6((LCD_WIDTH>>1)-(strlen(popup_text3)*3),110,popup_text3,255,100,100);


	}
	

	uint16_t x;
	uint16_t y;
	if(check_button_press(&x,&y)==1)
	{
		if(type==1)
		{
			redraw=1;
			set_current_execution(current_execution);
		}
		else
		{
			
			if(x < 160)
			{
				response = 0;
			}
			else
			{
				response = 1;
			}
			
			redraw=1;
			set_current_execution(current_execution);
			
		}
	}
}


