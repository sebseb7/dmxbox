#include "dmx_device_class.h"



/*
typedef struct {

	uint8_t name_length;
	char* name;
	uint8_t channels;
	uint8_t channels_allocated;
	uint8_t *channel_defaults; 
	int16_t *channel_defaults_blackout; // will be called when blackout is issued , -1 means , do not overwrite
	int16_t *channel_defaults_identify; // will be called when identify is issued , -1 means , do not overwrite
	int16_t *channel_defaults_fullbright; // will be called when fullbright is issued , -1 means , do not overwrite
	char **channel_names;

} dmx_device_class_t;
*/


static uint16_t number_of_devices=0;
static dmx_device_class_t* devices_classes[10];

void add_device_class(dmx_device_class_t* new_device)
{
	devices_classes[number_of_devices]=new_device;
	number_of_devices++;
}

uint16_t get_device_class_count()
{
	return number_of_devices;
}
dmx_device_class_t* get_device_class(uint16_t idx)
{
	return devices_classes[idx];
}

