#ifndef __MAIN_H
#define __MAIN_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_midi_core.h"

#include "stm32f4xx.h"
#include "lcd_a/stm32f4_discovery_lcd.h"

#define LCD_WIDTH  LCD_PIXEL_WIDTH          
#define LCD_HEIGHT LCD_PIXEL_HEIGHT     

void USB_Host_Handle(void);
void TimingDelay_Decrement(void);
void delay_ms(__IO uint32_t nTime);
void clearDisplay(void);
void clear_buttons(void);
int check_button_press(uint16_t* x,uint16_t* y);
void set_current_execution(void (*new_execution)(void));
void setLedXY(uint16_t x,uint16_t y, uint8_t r,uint8_t g,uint8_t b);
void getLedXY(uint16_t x, uint16_t y, uint8_t* r,uint8_t* g, uint8_t* b);
#endif
