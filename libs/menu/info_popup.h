#ifndef _INFO_POPUP_H
#define _INFO_POPUP_H

#include <stdint.h>


void invoke_info_popup(char*,char*,char*);
void invoke_yes_no_popup(char*,char*,char*);
uint8_t get_yes_no_response(void);

#endif

