#include "fileio.h"

#include "main.h"
#include "ff.h"
#include "microsd_spi.h"

static FATFS FATFS_Obj;


static int initialized = 0;


static void initialize(void)
{
	if(initialized==1) return;
	initialized=1;
	
	disk_initialize(0);
	delay_ms(1);
	int result = disk_initialize(0);
	draw_number_8x6(100, 100, result,4,'0',255,255,255);
	if(result == 0)
	{
		result = f_mount(&FATFS_Obj,"",1);
		draw_number_8x6(100, 110, result,4,'0',255,255,255);
	}
}

void save_buffer(char * filename,uint8_t * outbuffer,uint16_t outbuffersize)
{
	initialize();
	FIL file;

	int result = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
	draw_number_8x6(100, 120, result,4,'0',255,255,255);
	unsigned int bw=0;
	result = f_write(&file, outbuffer, outbuffersize, &bw);
	draw_number_8x6(100, 130, result,4,'0',255,255,255);
	draw_number_8x6(100, 140, bw,4,'0',255,255,255);
	result = f_close(&file);
	draw_number_8x6(100, 150, result,4,'0',255,255,255);

}
uint16_t get_file_size(char * filename)
{
	initialize();
	
	FILINFO fno;
	fno.lfname = 0;
	FRESULT result = f_stat(filename, &fno);
	draw_number_8x6(200, 100, result,4,'0',255,255,255);
	draw_number_8x6(200, 110, fno.fsize,4,'0',255,255,255);
	return fno.fsize;
}

void load_buffer(char * filename,uint8_t * inbuffer,uint16_t inbuffersize)
{
	initialize();
	FIL file;

	int result = f_open(&file, filename, FA_READ);
	draw_number_8x6(50, 120, result,4,'0',255,255,255);
	unsigned int br=0;
	result = f_read(&file, inbuffer, inbuffersize, &br);
	draw_number_8x6(50, 130, result,4,'0',255,255,255);
	draw_number_8x6(50, 140, br,4,'0',255,255,255);
	result = f_close(&file);
	draw_number_8x6(50, 150, result,4,'0',255,255,255);
}


