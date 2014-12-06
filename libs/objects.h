#ifndef _DEVICES_H
#define _DEVICES_H


// contains all devices like led-par's , fog-machines , ...


struct {

	uint8_t name_length;
	char* name;
	uint8_t channels;
	uint8_t *channel_defaults; 
	uint16_t *channel_defaults_blackout; // will be called when blackout is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_identify; // will be called when identify is issued , 0xffff means , do not overwrite
	uint16_t *channel_defaults_fullbright; // will be called when fullbright is issued , 0xffff means , do not overwrite
	char **channel_names;

} device_class_t

struct {

	device_class_t *device_class;
	uint16_t base_address;
	uint8_t name_length;
	char *name;

} devices_t;


/*
 * 	contains a specific set of channel values over one or more devices
 * 	and channles (references by deviceid and channelnumer
 * 	(not absolute channel numbers, so devices can be relocated)
 */

struct {

	uint8_t name_length;
	char* name;
	uint8_t sets_length;
	devices_t **set_device;
	uint8_t *set_channel;
	uint8_t *set_value;

} scene_t;


/*
 * contains a set of scenes which it will hold for 
 * a specific time and over which it will loop;
 *
 */

struct {

	uint8_t name_length;
	char* name;
	uint16_t scene_length;
	scene_t **scenes;
	uint16_t *hold_time;
	uint8_t *morph_to_next; // morph between scenes
	uint16_t repeats;

} loop_t;


/*
 * contains a number of loops which will be played consecutively 
 * (multiple programms can run in parallel)
 *
 */

struct {

	uint8_t name_length;
	char* name;
	uint16_t playback_length;
	loop_t **loops;

} playback_t;


/*
 * contains a number of scenes for single action 
 * (like fog, blinder)
 *
 */

struct {
	
	uint16_t action_id;
	uint8_t name_length;
	char* name;
	scene_t **scenes;
	uint8_t scene_priority;
	loop_t **loops;
	uint8_t loop_priority;
	uint8_t type; // continuous, specific time/length  or single action
	uint16_t duration; //if type is 'length'
	
} action_t;

/*
 *
 * maps midi keys to actions
 *
 */


struct {

	uint16_t mapping_id;
	uint8_t name_length;
	char* name;
	action_t *action;
	uint16_t input_id;

} button_mapping_t
	

/* todo: fader_mapping */

// a = (MY_TYPE) { .value = 234, .stuff = 1.234, .flag = false };

#endif
