#ifndef _DMX_DEVICE_CLASS_H
#define _DMX_DEVICE_CLASS_H


// contains all devices like led-par's , fog-machines , ...
#include <stdint.h>


typedef struct __attribute__((__packed__)) {

	char* name;
	uint32_t uuid;
	uint8_t channels;
	uint8_t channels_allocated;
	uint8_t *channel_defaults; 
	int16_t *channel_defaults_blackout; // will be called when blackout is issued , -1 means , do not overwrite
	int16_t *channel_defaults_identify; // will be called when identify is issued , -1 means , do not overwrite
	int16_t *channel_defaults_fullbright; // will be called when fullbright is issued , -1 means , do not overwrite
	char **channel_names;

} dmx_device_class_t;


void add_device_class(dmx_device_class_t*);
void del_device_class(uint8_t idx);
void del_all_device_classs(void);
uint16_t get_device_class_count(void);
dmx_device_class_t* get_device_class(uint16_t idx);
dmx_device_class_t* get_device_class_by_uuid(uint32_t uuid);


#endif
