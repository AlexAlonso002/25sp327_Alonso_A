

#define _GNU_SOURCE
#include <errno.h> 
#include <malloc.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <string.h>

#define handle_error(errnum,msg)
do{
    fprintf(stderr,"%s: (%s) %s\n" ,
    msg,sterrorname_np(errnum) , strerror(errnum)) ;
    exit(EXIT_FAILURE); 
} while(0)
void * isprime(void *data) ;
void *progress(void *data) ;
struct prime_result; 
void print_result(long long num, struct prime_result, )

int main(int argc , char*argcv[]) {
    long long number1 ; 
    long long number2; 
    if(argc != 3) {
        fprintf(stderr , "Enter 2 integer values") ;
        exit(EXIT_FAILURE) ;
    }
    number1 = atoll(argv[1]) ;
    number1 = atoll(argv[2]) ;  


    int rval ; 


    pthread_attr_t th_attr ; 
    if(pthread_attr_init(&th_attr) != 0 ){
        handle_error(rval, "pthread_attr_init") ;
    }
    if((rval = pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_JOINABLE)) != 0){
        handle_error(rval, "pthread_set_detach") ;
    }
   // pthread_attr_init(&attr) ;
    pthread_t tid_prime1; 
    pthread_t tid_prime2; 
    pthread_t tid_progress ; 
    pthread_attr_init(&th_attr) ; 
    if (( rval = pthread_create(&tid_prime1, &th_attr, isprime , &number1)) != 0){
        handle_error(rval, "pthread_set_crreate") ;
    }
    if (( rval = pthread_create(&tid_prime2, &th_attr, isprime , &number2)) != 0){
        handle_error(rval, "pthread_set_crreate") ;
    }
    if (( rval = pthread_create(&tid_progress, &th_attr, progress , NULL)) != 0){
        handle_error(rval, "pthread_set_crreate") ;
    }
    //pthread_create(&tid_prime2, &th_attr, isprime , &number2) ;

    pthread_join(tid_prime1,NULL) ;
    pthread_join(tid_prime2,NULL) ;

    pthread_detach(tid_progress) ; 
    pthread_join(tid_prime1, NULL) ;
    pthread_join(tid_prime2, NULL) ;

    struct prime_result *prime1 ; 
    struct prime_result *prime2 ;
    if((rval=pthread_join(tid_prime) != 0) {
        
    })

    if ((rval = pthread_cancel((tid_progress)) != 0)){
        handle_error(rval, "pthread_set_cancel") ;
    }
    pthread_attr_destroy(&th_attr) ; 
    printf("Done \n") ;
    return 0 ;
}

void * isprime(void *data){
    long long num = *((long long *)) data ;
    printf("HI %lld\n " , num) ;
    int prime = 1 ;
    for(long long j =2 ; j < num ; j++){
        if (num % 2 == 0) {
            prime = 0
        }
    }
    printf("\n%lld is %s.\n",num , prime ? "prime" : "composite")
    pthread_exit(NULL) ;
    
}

void * progress(void *data){
    while(1){
        sleep(1) ;
        printf(".") ;
        fflush(stdout) ;
    }
    pthread_exit(NULL) ;

}