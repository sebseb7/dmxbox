#include "fileio.h"

#include <stdio.h>
#include <sys/stat.h>

void save_buffer(char * filename,uint8_t * outbuffer,uint16_t outbuffersize)
{
	printf("save %s\n",filename);
	FILE * dmxfile =  fopen(filename, "wb");
	fwrite(outbuffer, 1,outbuffersize , dmxfile);
	fclose(dmxfile);
}
uint16_t get_file_size(char * filename)
{
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

void load_buffer(char * filename,uint8_t * inbuffer,uint16_t inbuffersize)
{
	printf("load %s\n",filename);
	FILE * dmxfile =  fopen(filename, "rb");
	fread(inbuffer, 1,inbuffersize , dmxfile);
	fclose(dmxfile);
}


