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

	uint16_t inbuffersize = get_file_size(filename);

	uint8_t* inbuffer=my_malloc(inbuffersize);
	load_buffer(filename,inbuffer,inbuffersize);
	uint16_t ptr=0;

	uint16_t count = inbuffer[ptr];ptr++;
	
	for(uint16_t i = 0;i < count;i++)
	{
		dmx_device_class_t *new_device_class=my_malloc(sizeof(dmx_device_class_t));
		
		new_device_class->channels=inbuffer[ptr];ptr++;
		new_device_class->channels_allocated=new_device_class->channels;
		
		uint8_t namelen = inbuffer[ptr];ptr++;
		new_device_class->name=my_malloc(namelen+1);
		memcpy(new_device_class->name,&inbuffer[ptr],namelen);ptr+=namelen;
		new_device_class->name[namelen]=0;

		
		new_device_class->channel_defaults=my_malloc(new_device_class->channels*sizeof(uint8_t));
		memcpy(new_device_class->channel_defaults,&inbuffer[ptr],new_device_class->channels*sizeof(uint8_t));ptr+=new_device_class->channels*sizeof(uint8_t);

		new_device_class->channel_defaults_blackout=my_malloc(new_device_class->channels*sizeof(int16_t));
		memcpy(new_device_class->channel_defaults_blackout,&inbuffer[ptr],new_device_class->channels*sizeof(uint16_t));ptr+=new_device_class->channels*sizeof(uint16_t);

		new_device_class->channel_defaults_identify=my_malloc(new_device_class->channels*sizeof(int16_t));
		memcpy(new_device_class->channel_defaults_identify,&inbuffer[ptr],new_device_class->channels*sizeof(uint16_t));ptr+=new_device_class->channels*sizeof(uint16_t);

		new_device_class->channel_defaults_fullbright=my_malloc(new_device_class->channels*sizeof(int16_t));
		memcpy(new_device_class->channel_defaults_fullbright,&inbuffer[ptr],new_device_class->channels*sizeof(uint16_t));ptr+=new_device_class->channels*sizeof(uint16_t);

		new_device_class->channel_names=my_malloc(new_device_class->channels*sizeof(char*));

		for(int j=0;j<new_device_class->channels;j++)
		{
			uint8_t namelen2 = inbuffer[ptr];ptr++;
			new_device_class->channel_names[j]=my_malloc(namelen2+1);
			memcpy(new_device_class->channel_names[j],&inbuffer[ptr],namelen2);ptr+=namelen2;
			new_device_class->channel_names[j][namelen2]=0;
		}
		//printf("add device\n");
		add_device_class(new_device_class);
	}

	my_free(inbuffer);
	my_free(filename);
}

char** list_device_classes(void)
{
	char** x=NULL;
	return x;
}







