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

int main(int argc, char* argv[]) {
    int num_threads = 2;  // Default number of threads
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    // Create socket for the server
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Set socket options to reuse address
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error setting SO_REUSEADDR");
        exit(1);
    }

    // Prepare server address structure
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(LISTEN_PORT);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, 5);  // Listen for incoming connections
    clilen = sizeof(cli_addr);

    while (1) {
        // Accept a new client connection
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Error on accept");
            exit(1);
        }

        // Allocate memory for the client socket pointer
        int *client_socket_ptr = malloc(sizeof(int));
        if (!client_socket_ptr) {
            perror("malloc client socket");
            continue;
        }

        *client_socket_ptr = newsockfd;  // Set the client socket pointer

        // Allocate memory for the request_t structure
        request_t* req = malloc(sizeof(request_t));
        if (!req) {
            perror("malloc request");
            free(client_socket_ptr);  // Free the client socket pointer if request_t malloc fails
            continue;
        }

        // Set up the request_t structure
        req->work_fn = handle_client_request_thread;
        req->arg = client_socket_ptr;

        // Directly handle the client request (blocking call)
        req->work_fn(req->arg);

        // Free the request struct after handling the request
        free(req);
    }
    close(sockfd);

    return EXIT_SUCCESS;

}