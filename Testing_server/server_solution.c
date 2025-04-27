#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Avoid socket in use errors
    // To manipulate options at the  sockets  API  level, level is specified as SOL_SOCKET.
    int optval = 1;  // Basically a boolean to enable/disable the option.
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0) {
        perror("ERROR setting socket options");
        exit(EXIT_FAILURE);
    }

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(EXIT_FAILURE);
    }

    /* Initialize socket structure */
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */

    if (listen(sockfd, 5) < 0) {
        perror("ERROR on listen");
        exit(EXIT_FAILURE);
    }

    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(EXIT_FAILURE);
    }

    // Convert IP address to string
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(cli_addr.sin_addr), ip_str, sizeof(ip_str));

    fprintf(stderr, "Accepted connection from %s:%d\n", ip_str, ntohs(cli_addr.sin_port));

    /* If connection is established then start communicating */
    memset(buffer, 0, sizeof(buffer));
    n = recv(newsockfd, buffer, sizeof(buffer) - 1, 0);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(EXIT_FAILURE);
    }

    printf("Here is the message: %s\n", buffer);

    /* Write a response to the client */
    n = send(newsockfd, "I got your message", 18, 0);

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(EXIT_FAILURE);
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}