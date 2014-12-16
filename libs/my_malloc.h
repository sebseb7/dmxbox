#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#include <stdint.h>
#include <stddef.h>


void *my_malloc (size_t nSize);
void  my_free (void *pMem);
void* my_realloc(void* ptr, size_t size);


#endif

