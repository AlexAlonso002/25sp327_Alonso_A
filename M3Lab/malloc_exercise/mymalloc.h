#include <stdlib.h> 
#ifndef MYMALLOC_H
#define MYMALLOC_H

void *alloc_bytes(size_t num_bytes) ; 

void *realloc_bytes(void* ptr, size_t num_bytes);

#endif