#ifndef _FILEIO_H
#define _FILEIO_H


#include <stdint.h>


void save_buffer(char * filename,uint8_t * outbuffer,uint16_t outbuffersize);
uint16_t get_file_size(char * filename);
void load_buffer(char * filename,uint8_t * inbuffer,uint16_t inbuffersize);



#endif
