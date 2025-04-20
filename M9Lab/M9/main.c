#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <stdio.h>
#include "common.h"
#include "queue.h"
#include "tasks.h"
#include "thread_pool.h"

// HINT: USE CH 15 CV_SIMPLE 

static request_t* create_request(int task_type);
static void random_sleep_time_in_range(int min_ms, int max_ms, struct timespec* ts);
static void nap_random();

int main(int argc, char* argv[]) {
    printf("Hi\n");

    (void)argc;
    (void)argv;
    
    if (argc < 3) {
        // printf("We need 2 arguments. One for number of threads. Seconds for number of requests\n");
        exit(0);
    }

    int num_threads = 0;
    int num_requests = 0;
    num_threads = atoi(argv[1]);  
    if(num_threads > 20){
        printf("Must be less than 20 threads") ;
        exit(0) ; 
    }
    num_requests = atoi(argv[2]);

    printf("Num of threads %d , num of requests %d\n", num_threads, num_requests);

    // seed random number to vary results between program executions
    srand(time(NULL));

    // Initialize mutex and cond variables
    pthread_mutex_t req_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t req_cond = PTHREAD_COND_INITIALIZER;

    // Initialize queue and thread pool
    queue_t* que = queue_init(&req_mutex, &req_cond);
    struct thread_pool* thread_pool = thread_pool_init(que, num_threads);

    int count = 1;
    // send all the request into the queue
    while (count <= num_requests) {
        int task_type = rand() % 3;
        queue_enqueue(que, create_request(task_type));
        nap_random();
        count++;
    }
    queue_close(que);
    thread_pool_destroy(thread_pool);
    queue_destroy(que);
    printf("Everything is destroy \n");
    // Perform cleanup tasks here
    pthread_exit((void*)0);
}

// Sleep for random milliseconds in range [50, 500]
static void nap_random() {
    struct timespec sleep_time;
    random_sleep_time_in_range(50, 500, &sleep_time);
    nanosleep(&sleep_time, NULL);
}

// A struct timespec with a random sleep time in the given range.
// @param min_ms Minimum number of milliseconds (inclusive)
// @param max_ms Maximum number of milliseconds (inclusive)
// @param ts Pointer to struct timespec to be filled
static void random_sleep_time_in_range(int min_ms, int max_ms, struct timespec* ts) {
    if (min_ms > max_ms || min_ms < 0) {
        // Invalid range, default to 0
        min_ms = max_ms = 0;
    }
    int millis = min_ms + rand() % (max_ms - min_ms + 1);
    ts->tv_sec = millis / 1000;
    ts->tv_nsec = (millis % 1000) * 1000000L;
}

// Creates a request of the given task_type
// @param task_type
// @return a populated request
static request_t* create_request(int task_type) {
    static int msg_count = 1;
    request_t* r = malloc(sizeof(request_t));

    if (r == NULL) {
        fprintf(stderr, "Memory allocation failed for request_t\n");
        return NULL;
    }

    switch (task_type) {
        case 0:
            {
                char* msg = malloc(64);
                if (msg == NULL) {
                    fprintf(stderr, "Memory allocation failed for message\n");
                    free(r);
                    return NULL;
                }
                snprintf(msg, 64, "Log message %d", msg_count++);
                r->work_fn = write_log;
                r->arg = msg;
                return r;
            }
        case 1:
            {
                int* val = malloc(sizeof(int));
                if (val == NULL) {
                    fprintf(stderr, "Memory allocation failed for int\n");
                    free(r);
                    return NULL;
                }
                *val = rand() % 1000;
                r->work_fn = compute_sum;
                r->arg = val;
                return r;
            }
        case 2:
            {
                struct timespec* delay = malloc(sizeof(struct timespec));
                if (delay == NULL) {
                    fprintf(stderr, "Memory allocation failed for delay\n");
                    free(r);
                    return NULL;
                }
                random_sleep_time_in_range(25, 250, delay);
                r->work_fn = delay_task;
                r->arg = delay;
                return r;
            }
        default:
            free(r);
            fprintf(stderr, "Unknown task type %d.\n", task_type);
            return NULL;
    }
}
