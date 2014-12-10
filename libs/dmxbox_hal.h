#ifndef __DMXBOX_HAL_H
#define __DMXBOX_HAL_H

#include <stdint.h>

void clearDisplay(void);
void clear_buttons(void);
int check_button_press(uint16_t* x,uint16_t* y);
void clear_buttons_midi(void);
void clear_faders(void);
int check_button_press_midi(uint8_t* cc,uint8_t* value);
void set_current_execution(void (*new_execution)(void));
void (*get_current_execution(void))(void);
uint8_t MIDI_get_fader(uint8_t ch);
void MIDI_send_cc(uint8_t cc,uint8_t value);
uint8_t MIDI_get_fader_updated(uint8_t ch);
#endif
