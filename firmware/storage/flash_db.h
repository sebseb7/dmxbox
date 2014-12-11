#ifndef FLASH_DB_H_
#define FLASH_DB_H_

#include "eeprom.h"

enum {
	ADDR_TETRIS_HIGHSCORE,
	ADDR_JOYCAL_MAX_X,
	ADDR_JOYCAL_MIN_X,
	ADDR_JOYCAL_CENTER_X,
	ADDR_JOYCAL_MAX_Y,
	ADDR_JOYCAL_MIN_Y,
	ADDR_JOYCAL_CENTER_Y
};


void flash_db_read(uint16_t addr,uint16_t *data);
void flash_db_write(uint16_t addr,uint16_t data);

#endif
