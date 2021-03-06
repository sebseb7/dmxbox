#ifndef _DMX_DEVICE_H
#define _DMX_DEVICE_H


#include <stdint.h>
#include "dmx_device_class.h"

typedef struct  __attribute__((__packed__)){

	uint32_t uuid;
	uint32_t device_class_uuid;
	uint16_t base_address;
	char * name;

} dmx_device_t;



void add_device(dmx_device_t*);
void del_device(uint8_t idx);
void del_all_devices(void);
uint16_t get_device_count(void);
dmx_device_t* get_device(uint16_t idx);
dmx_device_t* get_device_by_uuid(uint32_t uuid);


#endif
