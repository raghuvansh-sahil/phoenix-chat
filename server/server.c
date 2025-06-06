#include <stdio.h>       
#include <stdlib.h>
#include <unistd.h>      // Provides access to the POSIX API — includes close(), read(), write(), fork(), etc.
#include <errno.h>       // Defines the errno variable and error codes.
#include <string.h>      
#include <sys/types.h>   // Defines data types used in system calls like pid_t, size_t, ssize_t.
#include <sys/socket.h>  // Provides socket-related functions and constants: socket(), bind(), listen(), accept(), etc.
#include <netinet/in.h>  // Defines structures for internet domain addresses, like struct sockaddr_in.
#include <netdb.h>       // Used for DNS lookups and hostname translation (getaddrinfo, gethostbyname, etc).
#include <arpa/inet.h>   // Functions for IP address conversion, like inet_ntop, inet_pton.

#include "../common/utils.h"

#define PORT "21111"
#define BACKLOG 50

void get_listening_socket(int *sockfd) {
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if (bind(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*sockfd);
            perror("server: bind");
            continue;
        }

        break; 
    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);
}

void get_connecting_sockets(int sockfd, int *new_fd) {
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("server: waiting for connections...\n");

    while (1) {
        sin_size = sizeof their_addr;
        *new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (*new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
    }
}