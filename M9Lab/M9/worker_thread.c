#include "worker_thread.h"
#include <string.h>
#include "common.h"
void* do_work(void* worker_thread_params) {
    struct worker_thread_params* params = (struct
    worker_thread_params*)worker_thread_params;
    fprintf(stderr, "thread-%d starting\n", params->thread_id);
    // queue_t* que = params->que;
    int req_count = 0;
    request_t* req = NULL;

    do {
        // work until dequeue return null aka everything is done
        req = queue_dequeue(params->que);
        if (req == NULL) {
            break; 
        }
        // do the task
        req->work_fn(req->arg); 
        req_count++;
        // free req due to it being a pointer 
        free(req); 
    } while (1);
        fprintf(stderr, "thread-%d exiting. Processed %d requests.\n", params->thread_id, req_count);
        // free the data
        free(params);
        pthread_exit((void*)0);
}