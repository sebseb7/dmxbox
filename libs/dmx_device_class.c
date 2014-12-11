#include "dmx_device_class.h"

#include "my_malloc.h"


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

void del_device_class(uint8_t idx)
{
	dmx_device_class_t *device_class = devices_classes[idx];
	for(int i=0;i<device_class->channels;i++)
	{
		my_free(device_class->channel_names[i]);
	}
	my_free(device_class->name);
	my_free(device_class->channel_defaults);
	my_free(device_class->channel_defaults_blackout);
	my_free(device_class->channel_defaults_identify);
	my_free(device_class->channel_defaults_fullbright);
	my_free(device_class->channel_names);
	my_free(device_class);

	number_of_devices--;
	for(int i = idx;i< number_of_devices;i++)
	{
		devices_classes[i]=devices_classes[i+1];
	}
}

