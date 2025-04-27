// common.h
/**
 * https://gcc.gnu.org/onlinedocs/cpp/Once-Only-Headers.html
 * If a header file happens to be included twice, the compiler 
 * will process its contents twice. This is very likely to cause an error, 
 * e.g. when the compiler sees the same structure definition twice. 
 * Even if it does not, it will certainly waste time.
 */
#ifndef COMMON_H
#define COMMON_H
// ```````````````````````LOOK AT CV SIMPLE ITS IN CHAPTER 15!!!!!!!!!````````````````````````

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define handle_error_en(en, msg)                         \
    do {                                                 \
        fprintf(stderr, "%s: (%d) %s\n", msg, en, strerror(en)); \
        exit(EXIT_FAILURE);                              \
    } while (0)

#define handle_error(msg)                                \
    do {                                                 \
        int en = errno;                                  \
        fprintf(stderr, "%s: (%s) %s\n",                 \
                msg, strerrorname_np(en), strerror(en)); \
        exit(EXIT_FAILURE);                              \
    } while (0)


#define LOCK_MTX(mtx) \
    do {             \
        int ret = 0 ; \
        if((ret = pthread_mutex_lock(mtx))) \
            handle_error_en(ret,"Pthread_Mutex_lock")  ;             \
    } while (0)       
 

#define UNLOCK_MTX(mtx) \
do {  \
    int ret = 0 ; \
        if((ret = pthread_mutex_unlock(mtx))) \
            handle_error_en(ret,"Pthread_Mutex_lock")  ; \
} while (0)                                             


typedef struct request {
    void (*work_fn)(void*);  // Pointer to a function that takes a void* and returns void
    void* arg;               // Argument to be passed to the work_fn
} request_t;    

#endif