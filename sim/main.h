#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#define SIMULATOR

enum {
	LCD_WIDTH = 320,
	LCD_HEIGHT= 240,
};

void clearDisplay(void);
void setLedXY(uint16_t x,uint16_t y, uint8_t r,uint8_t g,uint8_t b);
void invLedXY(uint16_t x, uint16_t y);
void getLedXY(uint16_t x, uint16_t y, uint8_t* r,uint8_t* g, uint8_t* b);
void clear_buttons(void);
int check_button_press(uint16_t* x,uint16_t* y);
void set_current_execution(void (*new_execution)(void));


#endif

