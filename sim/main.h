#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <stddef.h>

void *my_malloc (size_t nSize);
void  my_free (void *pMem);

#include "dmxbox_hal.h"
#include "pixel_hal.h"


#endif

