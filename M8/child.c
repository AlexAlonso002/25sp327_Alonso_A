#define _GNU_SOURCE
#define _POSIX_C_SOURCE 199309L  // Required for sigqueue()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t sum= 0; 

static void SigStop(){
    union sigval sum2; 
    sum2.sival_int = sum;
    printf("SIGTSTP received. Sending sum to parent.");
    sigqueue(getppid(), SIGUSR1, sum2); 
}

static void SigUser1(){
   printf("Daddy send me a text \n") ;
}

static void SigTerm(){
    union sigval value;
    value.sival_int = 1234; 
    printf("Im leaving daddy \n") ;
    sigqueue(getppid(), SIGCHLD, value); 
    exit(0) ;
}

int main(int argc, char **argv){
    struct sigaction stop;
    stop.sa_handler = SigStop;  
    sigaction(SIGTSTP, &stop, NULL);

    struct sigaction usr1;
    usr1.sa_sigaction = SigUser1; 
    usr1.sa_flags = SA_SIGINFO; 
    sigaction(SIGUSR1, &usr1, NULL);

    struct sigaction usr2;
    usr2.sa_handler = SigTerm;  
    sigaction(SIGTERM, &usr2, NULL);

    int value = getpid() ;
    printf("Child Process PID: %d\n", value);

    while(1){
        sum = sum + 5 ;
        printf("Running... Use 'kill -SIGTSTP %d' to interrupt me. \n" , value) ;
        sleep(2) ;
    }

    //union sigval sum2;
    //sum = sum +1 ;
    //sum2.sival_int = sum;
    //sigqueue(getppid(), SIGUSR1, sum2);
    //kill(getppid(),SIGUSR1) ; 


    //exit(0) ;
 
}