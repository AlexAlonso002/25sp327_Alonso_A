#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "common.h"
#include "http_proxy.h"
#include "queue.h"
#include "thread_pool.h"

#define LISTEN_PORT 8080  // Proxy listens on this port
// global var to stop while loop
volatile sig_atomic_t stop = 0 ;

void handle_sigint(int sig) {
    printf("\n Stopping \n") ;
    stop++ ; 
}
int main(int argc, char* argv[]) {
    int num_threads = 2;  // Default number of threads
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }
    pthread_mutex_t req_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t req_cond = PTHREAD_COND_INITIALIZER;
    // Initialize queue and thread pool
     queue_t* que = queue_init(&req_mutex, &req_cond);
      struct thread_pool* thread_pool = thread_pool_init(que, num_threads);

    // Create socket for the server
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    // create listener first
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // reuse server if being used
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error setting SO_REUSEADDR");
        exit(1);
    }
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(LISTEN_PORT);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    // listening
    listen(sockfd, 5);  
    clilen = sizeof(cli_addr);
    signal(SIGINT,handle_sigint);
    while (!stop ) {
        //connect to serve
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }
        // give this socket its own memory
        int *client_socket_ptr = malloc(sizeof(int));
        if (!client_socket_ptr) {
            perror("malloc client socket");
            continue;
        }

        *client_socket_ptr = newsockfd; 

        // Allocate memory for the request_t structure
        request_t* req = malloc(sizeof(request_t));
        //if fails try again
        if (!req) {
            perror("malloc request");
            free(client_socket_ptr);  
            continue;
        }

        // Set up the request_t structure
        req->work_fn = handle_client_request_thread;
        req->arg = client_socket_ptr;
        // send each called of request to the queue
        queue_enqueue(que , req) ;
    }

    // Clean up
    close(sockfd);
   // free(req) ;
    queue_close(que);
    thread_pool_destroy(thread_pool);
    queue_destroy(que);
    printf("Everything is destroy \n");
    pthread_exit((void*)0);

    return EXIT_SUCCESS;

}