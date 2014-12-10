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
#include "set_pixel_hal.h"
#include "menu_Callback.h"


#include "stm32f4xx.h"
#include "lcd_a/stm32f4_discovery_lcd.h"


void USB_Host_Handle(void);
void TimingDelay_Decrement(void);
void delay_ms(__IO uint32_t nTime);
#endif
