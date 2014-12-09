#ifndef _DMX_DEVICE_CLASS_H
#define _DMX_DEVICE_CLASS_H


// contains all devices like led-par's , fog-machines , ...


typedef struct {

	uint8_t name_length;
	char* name;
	uint8_t name_alloc;
	uint8_t channels;
	uint8_t *channel_defaults; 
	uint16_t *channel_defaults_blackout; // will be called when blackout is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_identify; // will be called when identify is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_fullbright; // will be called when fullbright is issued , 0xffff means , do not overwrite
	char **channel_names;

} dmx_device_class_t;


#endif
