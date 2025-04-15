#include "thread_pool.h"
#include "common.h"
struct thread_pool* thread_pool_init(queue_t* que, int num_threads) {
return NULL;
}

void thread_pool_destroy(struct thread_pool* pool) {
    if (!pool) return;

    // Wait for all worker threads to finish
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i].thread, NULL);
    }

    // Free the queue if it was dynamically allocated
    if (pool->que) {
        queue_destroy(pool->que);  // assuming you have this function
        pool->que = NULL;
    }

    // Free the pool structure itself
    free(pool);
}