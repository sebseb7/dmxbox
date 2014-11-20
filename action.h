#ifndef _ACTION_H
#define _ACTION_H


struct action_t {
	uint32_t id;
	char name[50];
	uint8_t priority; //if two zones/actions contain same scenes/devices higher priority overwrites lower priority

};






#endif

