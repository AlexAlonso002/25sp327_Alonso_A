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
   // printf("Enqueuing Node: %p (data = %p)\n", (void*)node, node->data);
    LOCK_MTX(que->mutex) ;
    que->tail-> next = node ; 
    que->tail = node ; 
    if (que->size == 0) {
        que->header->next = node;
        que->tail = node;
    } else {
        que->tail->next = node;
        que->tail = node;
    }
    que->size += 1 ;
    UNLOCK_MTX(que->mutex) ;
    // notify dequeue that something has been put into queue
    pthread_cond_broadcast(que->cond_var);    
}

void queue_close(queue_t* que){
   LOCK_MTX(que->mutex);  
    que->isclosed = true;
    UNLOCK_MTX(que->mutex);  
    // if any threads are waiting for enqueue kicked them off due to enqueue being done
    pthread_cond_broadcast(que->cond_var); 

}
  
bool queue_isclose(queue_t* que){
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

void* queue_dequeue(queue_t* que) {
    LOCK_MTX(que->mutex);
    // if there nothing in queue
    while (que->size == 0) {
        // if queueu is close bail and let others out
         if(queue_isclose(que) == true){
            printf("que is close freeing everyone \n") ;
            pthread_cond_broadcast(que->cond_var); 
            UNLOCK_MTX(que->mutex) ;
            return NULL ;
        }
        // else wait for enqueue to put stuff in
        else{
            pthread_cond_wait(que->cond_var, que->mutex);
        }
    }
    // move header to the next node
    queue_node_t* node = que->header->next;
    que->header->next = node->next;
    que->size -= 1;
    if (que->size == 0) {
        que->tail = que->header;
    }
    void* data = node->data;
    free(node);
    UNLOCK_MTX(que->mutex);
    // return task
    return data;
}

