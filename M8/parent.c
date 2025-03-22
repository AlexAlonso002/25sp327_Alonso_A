#define _GNU_SOURCE
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>


volatile sig_atomic_t sum = 0; 

static void SigChild(){
    printf("Child is Done ") ;

}

static void SigUser1(int sig, siginfo_t *sum2, void *context){
     sum = sum2->si_value.sival_int;
     printf("I Recieve" , sum)  ; 
}

static void SigUser2(){
    printf("Working") ;
}


int main(int argc, char **argv){   
    struct sigaction act;
    act.sa_flags = SA_NOCLDSTOP; 
    act.sa_handler = SigChild; 
    sigaction(SIGCHLD, &act, NULL);  

    struct sigaction usr1;
    usr1.sa_sigaction= SigUser1;  
    usr1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &usr1, NULL);

    struct sigaction usr2;
    usr2.sa_handler = SigUser2;  
    sigaction(SIGUSR2, &usr2, NULL);

    pid_t ret;
    ret = fork() ;
	if(ret == -1) {
        printf("fork failed!\n");
        exit(0) ;
    }
    else if(ret == 0){
        printf("Child \n") ;
        execl("./child", "./child", NULL);
    }
    else {
        printf("Parent \n");
        wait(NULL);
        printf("BYE \n") ;
    }

}