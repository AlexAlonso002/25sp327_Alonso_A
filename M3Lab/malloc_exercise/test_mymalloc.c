#include "mymalloc.h" 
#include <stdio.h> 
#include <malloc.h>

int main(void){
    printf("malloc_stats test driver\n") ;
    malloc_stats() ;
    char *ptr = alloc_bytes(512) ; 
    printf("\nAllocated 512 Bytes %p\n",ptr) ;

    malloc_stats(); 
}