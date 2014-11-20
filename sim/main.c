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
		printf("receive %i %i\n",last->x,last->y);
		*x = last->x;
		*y = last->y;
		remove_last();
		return 1;
	}
	return 0;
}
void clear_buttons()
{
	printf("clear ");
	while(last!=NULL)
	{
		printf(".");
		remove_last();
	}
	printf("\n");
}



static void (*current_execution)(void);
void set_current_execution(void (*new_execution)(void))
{
	current_execution = new_execution;
}

int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

	srand((unsigned int)time(NULL));

	SDL_Surface* screen;


	screen = SDL_SetVideoMode(LCD_WIDTH*2,LCD_HEIGHT*2,32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int running = 1;


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
									printf("got %i %i\n",x,y);
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
						printf("asda %i %i\n",ev.button.x>>1,ev.button.y>>1);
						add_button_press(ev.button.x>>1,ev.button.y>>1);
					}
					break;
				default: break;
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

	}

	SDL_Quit();
	return 0;
}

