#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void print_addrinfo(struct addrinfo *p);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    struct addrinfo* result;
    int rval = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (rval != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rval));
        exit(EXIT_FAILURE);
    }

    // Print for demo purposes
    print_addrinfo(result);

    // Try each address until we successfully connect
    struct addrinfo *p;
    int sockfd;
    for (p = result; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0)
            continue;

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0)
            break; // Success

        close(sockfd);
    }

    if (p == NULL) {
        fprintf(stderr, "ERROR connecting to host\n");
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    printf("Please enter the message: ");
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, sizeof(buffer) - 1, stdin);

    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);

    close(sockfd);
    return 0;
}

void print_addrinfo(struct addrinfo *p) {
    char ipstr[INET6_ADDRSTRLEN];

    for (struct addrinfo *current = p; current != NULL; current = current->ai_next) {
        void *addr;
        char *ipver;

        // Get pointer to the address itself
        if (current->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)current->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else if (current->ai_family == AF_INET6) { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)current->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        } else {
            printf("Unknown address family: %d\n", current->ai_family);
            continue;
        }

        // Convert the IP to a string and print it
        inet_ntop(current->ai_family, addr, ipstr, sizeof(ipstr));
        printf("  %s: %s\n", ipver, ipstr);
    }
}