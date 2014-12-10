#ifndef _SCREEN_KEYBOARD_H
#define _SCREEN_KEYBOARD_H

#include <stdint.h>

void invoke_keyboard(char*,char*);
char* get_keyboard_buffer(void);



void invoke_numeric_keyboard(char*,uint32_t);
uint32_t get_keyboard_number(void);

#endif

