#include "queue_internal.h"
#include <stdlib.h>
#include "queue.h"
#include "common.h"
#define _GNU_SOURCE

queue_t* queue_init(pthread_mutex_t* mutex, pthread_cond_t* cond_var) {
    queue_t* que = malloc(sizeof(queue_t)) ;
    if(!que){
        perror("queue_init") ;
        exit(EXIT_FAILURE) ;
    }
    que->mutex = mutex ; 
    que->cond_var = cond_var ; 
    que->isclosed = false ;
    que->size = 0 ;
 
    que->header = calloc(1,sizeof(queue_node_t)) ;
    if(!que->header) {
        perror("queue_init:header");
        exit(EXIT_FAILURE);
    }
    que->tail = que->header; 

    return que;

}



void queue_enqueue( queue_t* que , void* data) {
    // malloc a queue node and give it data
    queue_node_t* node = malloc(sizeof(queue_node_t)) ;
    node->data = data; 
    node->next = NULL ; 
    LOCK_MTX(que->mutex) ;
    que->tail-> next = node ; 
    que->tail = node ; 
    if(que->size == 0){
        que->header->next = node ;
        printf("Size is %d \n" , que->size) ;
    }
    que->size += 1 ;
    UNLOCK_MTX(que->mutex) ;
}

void queue_close(queue_t* que){
    que->isclosed = true ; 
}
  
bool queue_isclose(queue_t* que){
    // return bol
    if(que->isclosed == true){
        return true ;
    }
    else{
        return false  ; 
    }
}

void queue_destroy(queue_t* que){
    free(que->header) ;
    free(que) ;
}

void queue_dequeue(queue_t* que){
    queue_node_t* node = que->header->next;
    printf("Dequeuing node with data: %d\n", *((int*)node->data));

    LOCK_MTX(que->mutex) ;
    que->header->next = node->next;
    que->size -= 1 ; 
    free(node);
    UNLOCK_MTX(que->mutex) ;
}

/* 
if (node == NULL) {
        printf("Node is" , que->header->next , " \n") ;
        UNLOCK_MTX(que->mutex);
        return;
    }

*/