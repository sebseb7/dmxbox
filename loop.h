#ifndef _LOOP_H
#define _LOOP_H

struct loop_entry_t {    

	uint32_t duration; // in ms
	uint8_t blend; // 0== switch ; 1==blend in with duration 
	uint32_t entries; // number od channels
	uint16_t *channels;	// 8 bit channel + 8 bit data
	
	struct loop_entry_t *next;
};


struct loop_t {    
	uint32_t id;
	char name[50];

	struct loop_entry_t *next;

};





#endif

