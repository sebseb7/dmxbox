#include "dmx_device.h"
#include "dmx_device_class.h"

#include "my_malloc.h"


/*
typedef struct  __attribute__((__packed__)){

	dmx_device_class_t *device_class;
	uint16_t base_address;
	char *name;

} dmx_device_t;
*/


static uint16_t number_of_devices=0;
static dmx_device_t* devices[50];

void add_device(dmx_device_t* new_device)
{
	devices[number_of_devices]=new_device;
	number_of_devices++;
}

uint16_t get_device_count()
{
	return number_of_devices;
}
dmx_device_t* get_device(uint16_t idx)
{
	// handle index out of bounds?
	return devices[idx];
}
dmx_device_t* get_device_by_uuid(uint32_t uuid)
{
	for(int i = 0;i< number_of_devices;i++)
	{
		if(devices[i]->uuid == uuid)
			return devices[i];
	}
	return NULL;
}

static void free_device(uint8_t idx)
{
	// handle index out of bounds?
	dmx_device_t *device = devices[idx];
	my_free(device->name);
	my_free(device);
}

void del_device(uint8_t idx)
{
	// handle index out of bounds?
	free_device(idx);

	number_of_devices--;
	for(int i = idx;i< number_of_devices;i++)
	{
		devices[i]=devices[i+1];
	}
}
void del_all_devices()
{
	for(int j=0;j<number_of_devices;j++)
	{
		free_device(j);
	}
	number_of_devices=0;
};

