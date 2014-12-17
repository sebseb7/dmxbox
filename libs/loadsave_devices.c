#include <stdio.h>//only for debug printf
#include <string.h>
#include "loadsave_devices.h"
#include "storage/fileio.h"
#include "dmx_device.h"
#include "my_malloc.h"


/*

typedef struct  __attribute__((__packed__)){

	uint32_t uuid;
	uint32_t device_class_uuid;
	uint16_t base_address;
	char * name;

} dmx_device_t;


*/

void save_devices(char* name)
{

	uint16_t count = get_device_count();

	uint16_t outbuffersize=1;

	for(uint16_t i = 0;i < count;i++)
	{
		outbuffersize++;
		outbuffersize+= strlen(get_device(i)->name);
		outbuffersize+=sizeof(uint32_t);
		outbuffersize+=sizeof(uint32_t);
		outbuffersize+=sizeof(uint16_t);
	}

	uint8_t* outbuffer=my_malloc(outbuffersize);
	uint16_t ptr=0;
	outbuffer[ptr]=count;
	ptr++;

	for(uint16_t i = 0;i < count;i++)
	{
		outbuffer[ptr]=strlen(get_device(i)->name);
		ptr++;
		
		memcpy(&outbuffer[ptr],get_device(i)->name,strlen(get_device(i)->name));
		ptr+=strlen(get_device(i)->name);
		
		memcpy(&outbuffer[ptr],&get_device(i)->uuid,sizeof(uint32_t));
		ptr+=sizeof(uint32_t);
		
		memcpy(&outbuffer[ptr],&get_device(i)->device_class_uuid,sizeof(uint32_t));
		ptr+=sizeof(uint32_t);
		
		memcpy(&outbuffer[ptr],&get_device(i)->base_address,sizeof(uint16_t));
		ptr+=sizeof(uint16_t);

	}

	//why +1 ? 
	char* filename = my_malloc(sizeof(char)*(strlen(name)+strlen("devices_.dmx")+1));

	snprintf(filename,strlen("devices_.dmx")+strlen(name)+1,"devices_%s.dmx",name);

	save_buffer(filename,outbuffer,outbuffersize);

	my_free(filename);
	my_free(outbuffer);

}

void load_devices(char* name)
{
	del_all_devices();

	// why +1 ??
	char* filename = my_malloc(sizeof(char)*(strlen(name)+strlen("devices_.dmx")+1));
	snprintf(filename,strlen("devices_.dmx")+strlen(name)+1,"devices_%s.dmx",name);

	uint16_t inbuffersize = get_file_size(filename);


	if(inbuffersize < 2)
	{
		my_free(filename);
		return;
	}
	

	uint8_t* inbuffer=my_malloc(inbuffersize);
	load_buffer(filename,inbuffer,inbuffersize);
	uint16_t ptr=0;



	uint16_t count = inbuffer[ptr];ptr++;
	
	for(uint16_t i = 0;i < count;i++)
	{
		dmx_device_t *new_device=my_malloc(sizeof(dmx_device_t));
		
		uint8_t namelen = inbuffer[ptr];ptr++;
		new_device->name=my_malloc(namelen+1);
		memcpy(new_device->name,&inbuffer[ptr],namelen);ptr+=namelen;
		new_device->name[namelen]=0;
		
		memcpy(&new_device->uuid,&inbuffer[ptr],sizeof(uint32_t));ptr+=sizeof(uint32_t);
		memcpy(&new_device->device_class_uuid,&inbuffer[ptr],sizeof(uint32_t));ptr+=sizeof(uint32_t);
		memcpy(&new_device->base_address,&inbuffer[ptr],sizeof(uint16_t));ptr+=sizeof(uint16_t);
		
		add_device(new_device);
	}

	my_free(inbuffer);
	my_free(filename);
}

char** list_devices(void)
{
	char** x=NULL;
	return x;
}







