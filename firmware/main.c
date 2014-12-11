/**
 ******************************************************************************
 * @file    main.c
 * @author	Xavier Halgand
 * @version
 * @date
 * @brief   Dekrispator main file
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "menu/menu_main.h"
#include "menu/menu_setup.h"
#include "lcd_a/stmpe811qtr.h"

#include "tlsf.h"
#include "my_malloc.h"

#define POOL_SIZE 1024 * 28
char pool[POOL_SIZE];

uint8_t pool_init=0;
void *my_malloc(size_t size)
{
	if(pool_init==0)
	{
		pool_init=1;
		//printf("init\n");
		init_memory_pool(POOL_SIZE, pool);
	}
	//printf("malloc %i %i %i\n",(uint32_t)size,get_used_size(pool),get_max_size(pool));
	return malloc_ex(size, pool);
}
void my_free(void *ptr)
{
	free_ex(ptr, pool);
}

/*---------------------------------------------------------------------------*/

__ALIGN_BEGIN USB_OTG_CORE_HANDLE		USB_OTG_Core_dev __ALIGN_END  ;

__ALIGN_BEGIN USBH_HOST					USB_Host __ALIGN_END ;

/*====================================================================================================*/

static __IO uint32_t TimingDelay;
void delay_ms(__IO uint32_t nTime)
{
	TimingDelay = nTime*1;

	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

static uint16_t usb_ready=0;
void USB_Host_Handle()
{
	if(usb_ready==1)
	{
			USBH_Process(&USB_OTG_Core_dev , &USB_Host);
	}
}

static void (*current_execution)(void);
void set_current_execution(void (*new_execution)(void))
{
	current_execution = new_execution;
}
void (*get_current_execution(void))(void)
{
	return current_execution;
}

static uint16_t leds[LCD_HEIGHT-100][LCD_WIDTH];
static uint16_t leds_a[100][LCD_WIDTH] __attribute__((section(".ccm")));

	

void clearDisplay()
{
	int x, y;
	for(x = 0; x < LCD_WIDTH; x++) {
		for(y = 0; y < LCD_HEIGHT; y++) {
			if(y < (LCD_HEIGHT-100))
			{
				leds[y][x] = 0;
			}else{
				leds_a[y-140][x] = 0;
			}
//			leds[y][x][1] = 0;
//			leds[y][x][2] = 0;
		}
	}
}

void setLedXY(uint16_t x, uint16_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	if(y < (LCD_HEIGHT-100))
	{
		leds[y][x] = (( (red   >> 3) & 0x001f ) << 11 | ( (green >> 2) & 0x003f ) << 5 | ((blue  >> 3) & 0x001f));
	}
	else
	{
		leds_a[y-140][x] = (( (red   >> 3) & 0x001f ) << 11 | ( (green >> 2) & 0x003f ) << 5 | ((blue  >> 3) & 0x001f));
	}
//	leds[y][x][1] = green;
//	leds[y][x][2] = blue;
}

void invLedXY(uint16_t x, uint16_t y) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
//	leds[y][x][0] = 255 - leds[y][x][0];
//	leds[y][x][1] = 255 - leds[y][x][1];
//	leds[y][x][2] = 255 - leds[y][x][2];
}
void getLedXY(uint16_t x, uint16_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	
	if(y < (LCD_HEIGHT-100))
	{
		*red = (uint8_t) (leds[y][x] >> 11)<<3;
		*green = (uint8_t) (leds[y][x] >> 5) << 2;
		*blue = (uint8_t) leds[y][x] << 3;
	}else{
		*red = (uint8_t) (leds_a[y-140][x] >> 11)<<3;
		*green = (uint8_t) (leds_a[y-140][x] >> 5) << 2;
		*blue = (uint8_t) leds_a[y-140][x] << 3;
	}
	
}

struct button_press_t {    

	uint16_t x;
	uint16_t y;
	
	struct button_press_t *next;
	struct button_press_t *last;
};
static struct button_press_t *current;
static struct button_press_t *last;

static void add_button_press(uint16_t x,uint16_t y)
{

	struct button_press_t *p;

	p = (struct button_press_t *)my_malloc(sizeof(struct button_press_t));

	p->x = x;
	p->y = y;
	p->last=NULL;

	if(current != NULL)
	{
		current->last=p;
		p->next = current;
	}else
	{
		p->next=NULL;
		last=p;
	}

	current=p;
}
static void remove_last(void)
{
	if(last == current)
	{
		my_free(last);
		current=NULL;
		last=NULL;
	}
	else
	{
		struct button_press_t *p = last->last;
		my_free(last);
		last=p;
		last->next=NULL;
	}
}

struct midi_button_press_t {    

	uint8_t cc;
	uint8_t value;
	
	struct midi_button_press_t *next;
	struct midi_button_press_t *last;
};
static struct midi_button_press_t *current_midi;
static struct midi_button_press_t *last_midi;

static void add_midi_button_press(uint8_t cc,uint8_t value)
{

	struct midi_button_press_t *p;

	p = (struct midi_button_press_t *)my_malloc(sizeof(struct midi_button_press_t));

	p->cc = cc;
	p->value = value;
	p->last=NULL;

	if(current_midi != NULL)
	{
		current_midi->last=p;
		p->next = current_midi;
	}else
	{
		p->next=NULL;
		last_midi=p;
	}

	current_midi=p;
}
static void remove_last_midi(void)
{
	if(last_midi == current_midi)
	{
		my_free(last_midi);
		current_midi=NULL;
		last_midi=NULL;
	}
	else
	{
		struct midi_button_press_t *p = last_midi->last;
		my_free(last_midi);
		last_midi=p;
		last_midi->next=NULL;
	}
}
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;

//	init_heap();


	RCC_GetClocksFreq(&RCC_Clocks);
	/* SysTick end of count event each 0.1ms */
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);


	USBH_Init(&USB_OTG_Core_dev,
			USB_OTG_FS_CORE_ID,
			&USB_Host,
			&MIDI_cb,
			&USR_Callbacks);

	usb_ready=1;


	STM32f4_Discovery_LCD_Init();
	LCD_SetCursor(0x00, 0x00); 
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */


	current_execution = menu_main;

	TS_STATE *pstate = NULL;
	IOE_Config();

	//int oldx;
	//int oldy;
	int pressed=0;
	while(1)
	{
		current_execution();
		
		//draw_filledRect(180,20,100,10,60,60,60);
		//draw_number_8x6(182, 19,get_used_size(pool),10,'0',255,255,255);
		
		for(int y = 0; y < LCD_PIXEL_HEIGHT; y++) 
		{

			for(int x = 0; x < LCD_PIXEL_WIDTH; x++) 
			{

				
				if(y < (LCD_HEIGHT-100))
				{
					uint8_t red = (uint8_t) (leds[y][x] >> 11)<<3;
					uint8_t green = (uint8_t) (leds[y][x] >> 5) << 2;
					uint8_t blue = (uint8_t) leds[y][x] << 3;
			
					LCD_WriteRAM(red*0x100+green);
					LCD_WriteRAM(blue);
				}
				else
				{
					uint8_t red = (uint8_t) (leds_a[y-140][x] >> 11)<<3;
					uint8_t green = (uint8_t) (leds_a[y-140][x] >> 5) << 2;
					uint8_t blue = (uint8_t) leds_a[y-140][x] << 3;
				
					LCD_WriteRAM(red*0x100+green);
					LCD_WriteRAM(blue);
				}
	
			}
		}
		pstate = IOE_TS_GetState();
		if((pstate->TouchDetected)&&(pressed==0))
		{
			int x = pstate->X-350;
			if(x<0) x=0;
			int y = pstate->Y-420;
			if(y<0) y=0;
			y/=13.33f;
			x/=10.62f;
			x=320-x;

			add_button_press(x, y);
			/*draw_number_8x6(100, 100, oldx,4,'0',0,0,0);
			draw_number_8x6(100, 150, oldy,4,'0',0,0,0);
			draw_number_8x6(100, 100, x,4,'0',255,255,255);
			draw_number_8x6(100, 150, y,4,'0',255,255,255);*/

			//oldx=x;
			//oldy=y;
			pressed=1;
		}
		else if(! (pstate->TouchDetected))
		{
			pressed=0;
		}
	}

}


static uint8_t midi_fader[8];
static uint8_t fader_updated[8];
void MIDI_recv_cb(MIDI_EventPacket_t packet)
{
	if(packet.cc < 8)
	{
		midi_fader[packet.cc]=packet.value*2;
		if(midi_fader[packet.cc] > 127) midi_fader[packet.cc]++;
		fader_updated[packet.cc]=1;
	}
	//draw_number_8x6(100, 100, packet.cc,4,'0',255,255,255);

	if((packet.cc > 42)&&(packet.cc < 45))
	{
		add_midi_button_press(packet.cc,packet.value);
	}
	//MIDI_send(packet);
}

uint8_t MIDI_get_fader(uint8_t ch)
{
	if(ch > 7) return 0;
	return midi_fader[ch];
}
uint8_t MIDI_get_fader_updated(uint8_t ch)
{
	if(ch > 7) return 0;
	if(fader_updated[ch]==1)
	{
		fader_updated[ch]=0;
		return 1;
	}
	return 0;
}
			
void MIDI_send_cc(uint8_t cc,uint8_t value)
{
	MIDI_EventPacket_t packet;
	packet.channel = 0;
	packet.type = CC;
	packet.cc = cc;
	packet.value = value;
	MIDI_send(packet);
}


void clear_buttons()
{
	while(last!=NULL)
	{
		remove_last();
	}
}
void clear_faders()
{
	for(int i = 0 ; i < 8; i++)
		fader_updated[i]=0;
}
void clear_buttons_midi()
{
	while(last_midi!=NULL)
	{
		remove_last_midi();
	}
}

int check_button_press(uint16_t* x,uint16_t* y)
{
	if(last != NULL)
	{
		*x = last->x;
		*y = last->y;
		remove_last();
		return 1;
	}
	return 0;
}

int check_button_press_midi(uint8_t* cc,uint8_t* value)
{
	if(last_midi != NULL)
	{
		*cc = last_midi->cc;
		*value = last_midi->value;
		remove_last_midi();
		return 1;
	}
	return 0;
}


