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

    /**
 * @brief handle sigint signal to exit queueu
 * @param req_mutex = the signal

 */
void handle_sigint() {
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

    // TODO Initialize a queue and thread pool to handle requests
    /**
 * @brief Creating the queueu and thread pool
 * @param req_mutex = mutex pointer for the thread
 @param req_cond = condtion var to signal dequeue
 @param num_threads = num of threads we want to create

 */
     queue_t* que = queue_init(&req_mutex, &req_cond);
      struct thread_pool* thread_pool = thread_pool_init(que, num_threads);

   // TODO Create socket for the proxy server to listen on the LISTEN_PORT
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

// TODO Accept incoming client connections. Add a request_t to the queue to
    // handle the client's HTTP request. The client socket is the argument to be
    // passed to the handle_client_request_thread function.
    // Continue to accept client connections until SIGINT is received.

    struct sigaction exit2;
    memset(&exit2, 0, sizeof(exit2));
    exit2.sa_handler = handle_sigint;
     exit2.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &exit2, NULL) == -1) {
        perror("Signal failed to create \n");
        exit(1);
    }

    while (!stop ) {
        //connect to serve
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }
        // each socket needs it own mem for the threads 
        int *client_socket_ptr = malloc(sizeof(int));
        if (!client_socket_ptr) {
            perror("malloc client socket");
            continue;
        }
         *client_socket_ptr = newsockfd; 
        // Allocate memory for the request_t structure
        request_t* request = malloc(sizeof(request_t));
        if (!request) {
            perror("malloc request for request has failed \n");
            free(client_socket_ptr);  
            continue;
        }
        // create request
        request->work_fn = handle_client_request_thread;
        request->arg = client_socket_ptr;
        // send each called of request to the queue
        queue_enqueue(que , request) ;
    }

    // TODO perform any clean up before server shutdown
    close(sockfd);
    //free(request) ;
    queue_close(que);
    thread_pool_destroy(thread_pool);
    queue_destroy(que);
    printf("Everything is destroy \n");
    pthread_exit((void*)0);

    return EXIT_SUCCESS;

}
