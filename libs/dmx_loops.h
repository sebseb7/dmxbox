#ifndef _DMX_LOOPS_H
#define _DMX_LOOPS_H


// contains all devices like led-par's , fog-machines , ...
#include <stdint.h>


typedef struct __attribute__((__packed__)) {

	uint32_t device_uuid;
	uint16_t channel;
	uint16_t value;

} dmx_channelset_t;


typedef struct __attribute__((__packed__)) {

	char name[10];
	uint16_t hold_time_ms; //65sec max
	uint8_t channels;
	uint8_t channels_allocated;
	dmx_channelset_t *channelsets;

} dmx_step_t;


typedef struct __attribute__((__packed__)) {

	char* name;
	uint8_t steps_in_use;
	uint8_t steps_allocated;
	dmx_step_t *steps;

} dmx_loop_t;


void add_loop(dmx_loop_t*);
void del_loop(uint8_t idx);
void del_all_loops(void);
uint16_t get_loop_count(void);
dmx_loop_t* get_loop(uint16_t idx);

void loop_insert_step(dmx_loop_t*,dmx_step_t*,uint8_t position);
void loop_delete_step(dmx_loop_t*,uint8_t idx);
uint8_t loop_get_step_count(dmx_loop_t*);
dmx_step_t* loop_get_step(dmx_loop_t*,uint8_t idx);

void step_insert_channelset(dmx_step_t*, dmx_channelset_t*);
void step_delete_channelset(dmx_step_t*,uint8_t idx);
uint8_t step_get_channelset_count(dmx_step_t*);
dmx_channelset_t* step_get_channelset(dmx_step_t*,uint8_t idx);



#endif
