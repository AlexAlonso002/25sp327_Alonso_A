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
    printf("Working") ;
}

static void SigUser1(){
    union sigval sum2; 
    sum2.sival_int = sum;
    sigqueue(getppid(), SIGUSR1, sum2);  
}

static void SigUser2(){
    printf("Working") ;
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
    usr2.sa_handler = SigUser2;  
    sigaction(SIGUSR2, &usr2, NULL);

    int value = getpid() ;
    printf("Child Process PID: %d\n", value);

    kill(getppid(),SIGUSR1) ; 


    exit(0) ;
 
}