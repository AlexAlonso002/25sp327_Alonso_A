#include <stdio.h>
#include "mymalloc.h"

void *alloc_bytes(size_t num_bytes){
    void *ptr = NULL ; 
    ptr = malloc(num_bytes);
    if(!ptr){
        perror("alloc_bytes");
        fprintf(stderr, "alloc_bytes(%ld) Failed\n", num_bytes);
        exit(EXIT_FAILURE) ;
    }
    return ptr;
}

void *realloc_byes(void* ptr, size_t num_bytes){
    return NULL ;
}