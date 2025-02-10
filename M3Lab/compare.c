#include <stdlib.h>
#include <stdio.h> 
#include <malloc.h>

void *alloc_bytes(size_t num_bytes);

int main(void){
    printf("malloc_stats test driver\n") ;
    malloc_stats() ;
    char *ptr = alloc_bytes(1024) ; 
    printf("\nAllocated 1024 Bytes %p\n",ptr) ;

    //malloc_stats(); 
    printf("Success!\n");
}


void *alloc_bytes(size_t num_bytes){
    void *ptr = NULL ; 
    ptr = malloc(num_bytes);
    if(!ptr){
        perror("alloc_bytes");
        fprintf(stderr, "alloc_bytes(%ld) Failed\n", num_bytes);
        exit(EXIT_FAILURE) ;
    }
    free(ptr) ;
    ptr = NULL ;
    return ptr;
}