#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "main.h"
#include <string.h>
#include<sys/time.h>

#include "SDL.h"
#include "menu/menu_main.h"
#include "menu/menu_setup.h"

#include "keyboard.h"

static uint8_t leds[LCD_HEIGHT][LCD_WIDTH][4];

void clearDisplay()
{
	int x, y;
	for(x = 0; x < LCD_WIDTH; x++) {
		for(y = 0; y < LCD_HEIGHT; y++) {
			leds[y][x][0] = 0;
			leds[y][x][1] = 0;
			leds[y][x][2] = 0;
			leds[y][x][3] = 1;
		}
	}
}

void setLedXY(uint16_t x, uint16_t y, uint8_t red,uint8_t green, uint8_t blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	leds[y][x][0] = red;
	leds[y][x][1] = green;
	leds[y][x][2] = blue;
	leds[y][x][3] = 1;
}

void invLedXY(uint16_t x, uint16_t y) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	leds[y][x][0] = 255 - leds[y][x][0];
	leds[y][x][1] = 255 - leds[y][x][1];
	leds[y][x][2] = 255 - leds[y][x][2];
	leds[y][x][3] = 1;
}
void getLedXY(uint16_t x, uint16_t y, uint8_t* red,uint8_t* green, uint8_t* blue) {
	if (x >= LCD_WIDTH) return;
	if (y >= LCD_HEIGHT) return;
	*red = leds[y][x][0];
	*green = leds[y][x][1];
	*blue = leds[y][x][2];
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

	p = (struct button_press_t *)malloc(sizeof(struct button_press_t));

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
static void remove_last()
{
	if(last == current)
	{
		free(last);
		current=NULL;
		last=NULL;
	}
	else
	{
		struct button_press_t *p = last->last;
		free(last);
		last=p;
		last->next=NULL;
	}
}
int check_button_press(uint16_t* x,uint16_t* y)
{
	if(last != NULL)
	{
		//printf("receive %i %i\n",last->x,last->y);
		*x = last->x;
		*y = last->y;
		remove_last();
		return 1;
	}
	return 0;
}
void clear_buttons()
{
	//printf("clear ");
	while(last!=NULL)
	{
		//printf(".");
		remove_last();
	}
	//printf("\n");
}

	
MidiObj midi_korg;

void MIDI_send_cc(uint8_t cc,uint8_t value)
{
	keyboard_send(&midi_korg,176,cc,value);
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

	p = (struct midi_button_press_t *)malloc(sizeof(struct midi_button_press_t));

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
		free(last_midi);
		current_midi=NULL;
		last_midi=NULL;
	}
	else
	{
		struct midi_button_press_t *p = last_midi->last;
		free(last_midi);
		last_midi=p;
		last_midi->next=NULL;
	}
}

void clear_buttons_midi()
{
	while(last_midi!=NULL)
	{
		remove_last_midi();
	}
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
static uint8_t midi_fader[8];
static uint8_t fader_updated[8];
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
void clear_faders()
{
	for(int i = 0 ; i < 8; i++)
		fader_updated[i]=0;
}

int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	srand((unsigned int)time(NULL));

	SDL_Surface* screen;


	screen = SDL_SetVideoMode(LCD_WIDTH*2,LCD_HEIGHT*2,32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int running = 1;

	keyboard_init(&midi_korg,"nanoKONTROL");

	current_execution = menu_main;

	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							{
								uint16_t x;
								uint16_t y;
								if(check_button_press(&x,&y)==1)
								{
									//printf("got %i %i\n",x,y);
								}
							}
							break;
						case SDLK_1:
							clear_buttons();
							break;
						default: break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (ev.button.button == SDL_BUTTON_LEFT)
					{
						//printf("asda %i %i\n",ev.button.x>>1,ev.button.y>>1);
						add_button_press(ev.button.x>>1,ev.button.y>>1);
					}
					break;
				default: break;
			}
		}
		
		KeyboardEvent e;
		while(keyboard_poll(&midi_korg,&e)) 
		{
			if(e.x < 8)
			{
				midi_fader[e.x]=e.y*2;
				if(midi_fader[e.x] > 127) midi_fader[e.x]++;
				fader_updated[e.x]=1;
			}

			if((e.x > 42)&&(e.x < 45))
			{
				add_midi_button_press(e.x,e.y);
			}
		}


		current_execution();

		int x, y;
		for(x = 0; x < LCD_WIDTH; x++) {
			for(y = 0; y < LCD_HEIGHT; y++) {

				if(leds[y][x][3] == 1)
				{

					SDL_Rect rect = { x*2, y*2, 2,2 };
					SDL_FillRect(
							screen, 
							&rect, 
							SDL_MapRGB(screen->format, leds[y][x][0] & 0xFC ,leds[y][x][1] & 0xFC ,leds[y][x][2] & 0xFC)
							);
					leds[y][x][3] = 0;

				}

			}
		}



		SDL_Flip(screen);
		SDL_Delay(20);
	}

	SDL_Quit();
	return 0;
}

