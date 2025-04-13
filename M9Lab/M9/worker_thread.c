#include "worker_thread.h"
#include <string.h>
#include "common.h"
void* do_work(void* worker_thread_params) {
    struct worker_thread_params* params = (struct
    worker_thread_params*)worker_thread_params;
    fprintf(stderr, "thread-%d starting\n", params->thread_id);
    int req_count = 0;
    request_t* req = NULL;
    do {
    // TODO dequeue a request and process it.
    // A NULL value returned from dequeue means the thread should exit
    break;
    } while (1);
        fprintf(stderr, "thread-%d exiting. Processed %d requests.\n", params->thread_id, req_count);
        pthread_exit((void*)0);
}