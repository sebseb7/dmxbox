#ifndef _ZONE_H
#define _ZONE_H


struct zone_t {
	uint32_t id;
	char name[50];
	uint8_t priority; //if two zones/actions contain same scenes/devices higher priority overwrites lower priority

};






#endif

