#include "flash_db.h"


void flash_db_write(uint16_t addr,uint16_t data)
{


	FLASH_Unlock();
	EE_Init();
	EE_WriteVariable(addr, data);
	FLASH_Lock();

}


void flash_db_read(uint16_t addr,uint16_t *data)
{

	uint16_t read_data=0;

	FLASH_Unlock();
	EE_Init();
	EE_ReadVariable(addr, &read_data);
	FLASH_Lock();

	*data = read_data;

}
