#include <stdio.h>       
#include <stdlib.h>
#include <unistd.h>    
#include <errno.h>     
#include <string.h>      
#include <sys/types.h>   
#include <sys/socket.h> 
#include <netinet/in.h>  
#include <netdb.h>       
#include <arpa/inet.h>  

#include "../common/utils.h"
#include "command_parser.h"

#define PORT "21111"
#define BACKLOG 50

char *welcome_msg = "\nWelcome to PhoenixChat. Please authenticate using /connect <username>\n";

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

void get_connecting_sockets(int sockfd, fd_set *master, int *fdmax) {
    fd_set read_fds;
    int new_fd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    char buf[2048];
    int bytesReceived;

    int i, j;
    char s[INET6_ADDRSTRLEN];

    FD_ZERO(master);
    FD_ZERO(&read_fds);

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FD_SET(sockfd, master);
    *fdmax = sockfd; 

    printf("server: waiting for connections...\n");

    for (;;) {
        read_fds = *master;
        if (select(*fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i <= *fdmax; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sockfd) {
                    // handle new connections
                    sin_size = sizeof their_addr;
                    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
                    if (new_fd == -1) {
                        perror("accept");
                    }
                    else {
                        FD_SET(new_fd, master);
                        if (new_fd > *fdmax) {
                            *fdmax = new_fd;  
                        }
                        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
                        printf("server: new connection from %s on socket %d\n", s, new_fd);

                        // Send welcome message to the new client
                        send_message(new_fd, welcome_msg);
                    }
                }
                else {
                    // handle data from a client
                    if ((bytesReceived = receive_message(i, buf, sizeof buf)) <= 0) {
                        if (bytesReceived == 0) {
                            // connection closed
                            printf("server: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i);
                        FD_CLR(i, master);
                    } 
                    else {
                        // process the received message
                        printf("server: received '%s' from socket %d\n", buf, i);

                        Command *command = parse_command(buf);
                        printf ("server: parsed command: arg1='%s', arg2='%s', arg3='%s'\n", 
                               command->arg1 ? command->arg1 : "NULL", 
                               command->agr2 ? command->agr2 : "NULL", 
                               command->agr3 ? command->agr3 : "NULL");
                    }
                }
            }
        }
    }
}