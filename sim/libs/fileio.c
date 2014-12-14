#include "fileio.h"

#include <stdio.h>

void save_buffer(char * filename,uint8_t * outbuffer,uint16_t outbuffersize)
{
	printf("save %s\n",filename);
	FILE * dmxfile =  fopen(filename, "wb");
	fwrite(outbuffer, 1,outbuffersize , dmxfile);
	fclose(dmxfile);
}
uint16_t get_file_size(char * filename)
{
	return 0;
}

void load_buffer(char * filename,uint8_t * inbuffer,uint16_t inbuffersize)
{
	printf("load %s\n",filename);
	
}


