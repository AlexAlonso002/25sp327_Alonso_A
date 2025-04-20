#include "thread_pool.h"
#include "common.h"

// send all threads to the do work function
pthread_t create_worker_thread(struct worker_thread_params* params) {
    pthread_t tid;
    int result = pthread_create(&tid, NULL, do_work, params); 
    if (result != 0) {
        handle_error("create_worker_thread: pthread_create failed");
    }
    return tid;
}


struct thread_pool* thread_pool_init(queue_t* que, int num_threads) {
    struct thread_pool* pool = malloc(sizeof(struct thread_pool));
    if (!pool) {
        handle_error("thread_pool_init:malloc");
    }
    pool->next_thread_id = 1;
    pool->num_threads = num_threads;

    for (int i=0; i < num_threads; i++) {
        struct worker_thread_params* params = malloc(sizeof(struct worker_thread_params));
        params->que = que;
        params->thread_id = pool->next_thread_id;
        pool->threads[i] = malloc(sizeof(struct worker_thread));
        pool->threads[i]->thread_id = pool->next_thread_id; 
        pool->threads[i]->thread = create_worker_thread(params);
        pool->next_thread_id++;
    }

    return pool;
}

void thread_pool_destroy(struct thread_pool* pool) {
    if (!pool) {
        fprintf(stderr, "Error: thread pool is NULL.\n");
        return; 
    }
    // free each thread
    for (int i = 0; i < pool->num_threads; i++) {
        if (pool->threads[i]) {
            pthread_join(pool->threads[i]->thread, NULL);
            printf("Freeing thread %d...\n", pool->threads[i]->thread_id);
            free(pool->threads[i]); 
        }
    }
    printf("Freeing thread pool structure...\n");
    free(pool); 
}
