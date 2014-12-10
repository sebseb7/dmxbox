#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include <stddef.h>

void *xmalloc (size_t nSize);
void  xfree (void *pMem);

#include "dmxbox_hal.h"
#include "pixel_hal.h"


#endif

