#include <stdio.h>       
#include <stdlib.h>
#include <unistd.h>     
#include <string.h>      
#include <netdb.h>       

#include "utils.h"
#include "message.h"
#include "user_handler.h"
#include "command_parser.h"
#include "client_handler.h"

#define PORT "21111"
#define BACKLOG 50

char *welcome_msg = "\nWelcome to PhoenixChat. Please authenticate using /connect <username>\n";

void get_listening_socket(int *listening_socket) {
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "[SERVER] getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*listening_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("[SERVER] socket");
            continue;
        }

        if (setsockopt(*listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            close(*listening_socket);
            perror("[SERVER] setsockopt");
            continue;
        }

        if (bind(*listening_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(*listening_socket);
            perror("[SERVER] bind");
            continue;
        }

        break; 
    }

    if (p == NULL) {
        fprintf(stderr, "[SERVER] Failed to bind any address.\n");
        freeaddrinfo(servinfo);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servinfo);
}

void get_connecting_sockets(int listening_socket, fd_set *master, int *max_fd, User *logged_users[]) {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    if (listen(listening_socket, BACKLOG) == -1) {
        perror("[SERVER] listen");
        close(listening_socket);
        exit(EXIT_FAILURE);
    }

    FD_SET(listening_socket, master);
    *max_fd = listening_socket; 

    log_event("INFO", "Server started.");

    struct sockaddr_storage client_addr;
    char client_address[INET6_ADDRSTRLEN];

    for (;;) {
        read_fds = *master;
        if (select(*max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("[SERVER] select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= *max_fd; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listening_socket) { // handle new connections
                    socklen_t sin_size = sizeof client_addr;
                    int client_socket = accept(listening_socket, (struct sockaddr *)&client_addr, &sin_size);

                    if (client_socket == -1) {
                        perror("[SERVER] accept");
                    }
                    else {
                        FD_SET(client_socket, master);
                        if (client_socket > *max_fd) {
                            *max_fd = client_socket;  
                        }
                        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), client_address, sizeof client_address);
                        
                        char message[1024];
                        snprintf(message, sizeof message, "'%s' connected from socket %d.", client_address, client_socket);
                        log_event("CONNECT", message);
                        
                        User *client = create_user("", client_socket);
                        send_message(client, welcome_msg);
                        free_user(client);
                    }
                }
                else {  // handle data from a client
                    int temp_user = 0;

                    User *client = find_socket(logged_users, i);
                    if (!client) {
                        temp_user++;
                        client = create_user("", i);
                    }

                    int bytesReceived;
                    char buf[2048];

                    if ((bytesReceived = receive_message(client, buf, sizeof buf)) <= 0) {
                        if (bytesReceived == 0) {  // connection closed
                            char message[1024];
                            snprintf(message, sizeof message, "User '%s' hung up.", client->username);
                            log_event("DISCONNECT", message);
                        } 
                        else {
                            perror("[SERVER] recv");
                        }

                        close(i);
                        FD_CLR(i, master);
                    } 
                    else {
                        Command *cmd = parse_input(buf);
                        handle_client(cmd, client, logged_users);
                        free_command(cmd);
                    }

                    if (temp_user) {
                        free_user(client);
                    }
                }
            }
        }
    }
}