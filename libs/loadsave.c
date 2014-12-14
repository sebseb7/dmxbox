#include <stdio.h>//only for debug printf
#include <string.h>
#include "loadsave.h"
#include "storage/fileio.h"
#include "dmx_device_class.h"
#include "my_malloc.h"


/*
 *
 * typedef struct __attribute__((__packed__)) {

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

void save_device_classes(char* name)
{

	uint16_t count = get_device_class_count();

	uint16_t outbuffersize=1+(count*2);

	for(uint16_t i = 0;i < count;i++)
	{
		outbuffersize+= strlen(get_device_class(i)->name);
		outbuffersize+=sizeof(uint8_t)*get_device_class(i)->channels;
		outbuffersize+=sizeof(uint16_t)*get_device_class(i)->channels;
		outbuffersize+=sizeof(uint16_t)*get_device_class(i)->channels;
		outbuffersize+=sizeof(uint16_t)*get_device_class(i)->channels;
	
		for(uint16_t j=0;j<get_device_class(i)->channels;j++)
		{
			outbuffersize+=1;
			outbuffersize+=strlen(get_device_class(i)->channel_names[j]);
		}
	}

	uint8_t* outbuffer=my_malloc(outbuffersize);
	uint16_t ptr=0;
	outbuffer[ptr]=count;
	ptr++;

	for(uint16_t i = 0;i < count;i++)
	{
		outbuffer[ptr]=get_device_class(i)->channels;
		ptr++;
		
		outbuffer[ptr]=strlen(get_device_class(i)->name);
		ptr++;
		
		memcpy(&outbuffer[ptr],get_device_class(i)->name,strlen(get_device_class(i)->name));
		ptr+=strlen(get_device_class(i)->name);

		memcpy(&outbuffer[ptr],get_device_class(i)->channel_defaults,sizeof(uint8_t)*get_device_class(i)->channels);
		ptr+=sizeof(uint8_t)*get_device_class(i)->channels;
	
		memcpy(&outbuffer[ptr],get_device_class(i)->channel_defaults_blackout,sizeof(uint16_t)*get_device_class(i)->channels);
		ptr+=sizeof(uint16_t)*get_device_class(i)->channels;

		memcpy(&outbuffer[ptr],get_device_class(i)->channel_defaults_identify,sizeof(uint16_t)*get_device_class(i)->channels);
		ptr+=sizeof(uint16_t)*get_device_class(i)->channels;

		memcpy(&outbuffer[ptr],get_device_class(i)->channel_defaults_fullbright,sizeof(uint16_t)*get_device_class(i)->channels);
		ptr+=sizeof(uint16_t)*get_device_class(i)->channels;

		for(uint16_t j=0;j<get_device_class(i)->channels;j++)
		{
			outbuffer[ptr]=strlen(get_device_class(i)->channel_names[j]);
			ptr++;

			memcpy(&outbuffer[ptr],get_device_class(i)->channel_names[j],strlen(get_device_class(i)->channel_names[j]));
			ptr+=strlen(get_device_class(i)->channel_names[j]);
		}
	}

	//why +1 ? 
	char* filename = my_malloc(sizeof(char)*(strlen(name)+strlen("device_classes_.dmx")+1));

	snprintf(filename,strlen("device_classes_.dmx")+strlen(name)+1,"device_classes_%s.dmx",name);

	save_buffer(filename,outbuffer,outbuffersize);

	my_free(filename);
	my_free(outbuffer);

}

void load_device_classes(char* name)
{
	del_all_device_classs();

	// why +1 ??
	char* filename = my_malloc(sizeof(char)*(strlen(name)+strlen("device_classes_.dmx")+1));
	snprintf(filename,strlen("device_classes_.dmx")+strlen(name)+1,"device_classes_%s.dmx",name);
}

char** list_device_classes(void)
{
	char** x=NULL;
	return x;
}







