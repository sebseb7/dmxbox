#ifndef _DEVICES_H
#define _DEVICES_H


enum {
	DEVICE_FOG = 0,
	DEVICE_PAR36_LED,
	DEVICE_PAR36_LED_UV,
	DEVICE_PAR56_LED,
	DEVICE_QUADPHASE,
};



struct {

	uint16_t address;
	uint16_t type;
	


} devices_t


#endif
