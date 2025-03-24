#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>


volatile sig_atomic_t sum = 0; 


static void SigChild(int sig, siginfo_t *sum2, void *context){
    sum = sum2->si_value.sival_int;
    printf("PARENT: Child (PID: %d) has terminared with status 0. Exiting \n" , sum2->si_pid, sum)  ; 
    printf("PARENT: Child is Done \n") ;
    exit(0) ;

}

static void SigUser1(int sig, siginfo_t *sum2, void *context){
     sum = sum2->si_value.sival_int;
     printf("PARENT: Received SIGUSR1 from Child (PID: %d). Sum = %d \n" , sum2->si_pid, sum)  ;
    union sigval value;
    value.sival_int = 1; 
     if (sigqueue(sum2->si_pid, SIGUSR2, value) == -1) {
        perror("sigqueue failed");
        exit(1) ;
    } 
}



int main(int argc, char **argv){   
    // handling all signals handlers
    struct sigaction act;
    act.sa_flags = SA_NOCLDSTOP; 
    act.sa_sigaction = SigChild; 
    if( sigaction(SIGCHLD, &act, NULL) == -1) {
        perror("sigaction 4 SIGCHLD") ; 
        exit(1) ;
    }

    struct sigaction usr1;
    usr1.sa_sigaction= SigUser1;  
    usr1.sa_flags = SA_SIGINFO;
    if( sigaction(SIGUSR1, &usr1, NULL) == -1) {
        perror("sigaction 4 SIGUSR1 ") ; 
        exit(1);
    }

    pid_t ret;
    // create child 
    ret = fork() ;
	if(ret == -1) {
        printf("fork failed!\n");
        exit(1) ;
    }
    else if(ret == 0){
        // make child go to child.c
        printf("Child \n") ;
        execl("./child", "./child", NULL);
    }
    else {
        int status;
        printf("Parent\n");
        // wait until child dies
        while (waitpid(-1, &status, WNOHANG) == 0) {
            printf("PARENT: Working\n");
            sleep(3);  
        }
    }

}