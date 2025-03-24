#define _GNU_SOURCE
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
    printf("CHILD: SIGTSTP received. Sending sum to parent. \n");
    if(sigqueue(getppid(), SIGUSR1, sum2) == -1 ) {
        perror("sigqueue failed");
        exit(1) ;
    }
}

static void SigUser1(int sig, siginfo_t *sum2, void *context){
    int number ; 
    number = sum2->si_value.sival_int;
    printf("CHILD: Received SIGUSR2 from parent (PID: %d). Message = %d \n" , sum2->si_pid, number)  ;
}
static void SigTerm(){
    union sigval value;
    value.sival_int = 1; 
    printf("CHILD: Im leaving \n") ;
    if(sigqueue(getppid(), SIGCHLD, value) == -1){
        perror("sigqueue failed");
        exit(EXIT_FAILURE);
    } 
    exit(0) ;
}

int main(int argc, char **argv){
    // creating all signals handlers
    struct sigaction stop;
    stop.sa_handler = SigStop;  
    if(sigaction(SIGTSTP, &stop, NULL) == -1 ) {
        perror("sigaction failed for SIGSTP");
        exit(EXIT_FAILURE);
    }

    struct sigaction usr1;
    usr1.sa_sigaction = SigUser1; 
    usr1.sa_flags = SA_SIGINFO; 
    if(sigaction(SIGUSR2, &usr1, NULL) == -1){
        perror("sigaction failed for SIGUSR2");
        exit(EXIT_FAILURE); 
    }

    struct sigaction usr2;
    usr2.sa_handler = SigTerm;  
    if( sigaction(SIGTERM, &usr2, NULL) ==1){
         perror("sigaction failed for SIGTERM");
        exit(EXIT_FAILURE); 
    }

    int value = getpid() ;
    printf("Child Process PID: %d\n", value);
    // go forever printing this stuff until a kill signals
    while(1){
        sum = sum + 5 ;
        printf("CHILD: Running... Use 'kill -SIGTSTP %d' to interrupt me. \n" , value) ;
        sleep(2) ;
    }

    //union sigval sum2;
    //sum = sum +1 ;
    //sum2.sival_int = sum;
    //sigqueue(getppid(), SIGUSR1, sum2);
    //kill(getppid(),SIGUSR1) ; 


    //exit(0) ;
 
}