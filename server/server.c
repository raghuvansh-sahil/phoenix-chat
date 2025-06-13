#include <stdio.h>       
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>     
#include <string.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>    

#include "server.h"
#include "utils.h"
#include "user.h"
#include "command.h"
#include "message.h"
#include "hashing.h"
#include "input_parser.h"
#include "user_handler.h"

#define PORT "21111"
#define BACKLOG 50

static const char *welcome_msg = "\nWelcome to PhoenixChat. Please authenticate using /login <username>.";

static void handle_new_connections(int listening_socket, fd_set *master, int *max_fd) { 
    struct sockaddr_storage client_addr;
    socklen_t sin_size = sizeof client_addr;

    char client_address[INET6_ADDRSTRLEN];

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
                        
        User *client = create_user_with_socket(client_socket);
        send_message(client, welcome_msg, strlen(welcome_msg) + 1);
        free_user(client);
    }
}

static void handle_client_data(int client_socket, fd_set *master, User *logged_users[]) {
    User *client = create_user_with_socket(client_socket);
    char buf[1024];

    int bytesReceived;
    if ((bytesReceived = receive_message(client, buf, sizeof buf)) <= 0) {
        if (bytesReceived == 0) {  // connection closed
            char message[1024];
            snprintf(message, sizeof message, "User from socket %d hung up.", get_socket(client));
            log_event("DISCONNECT", message);
        }

        close(client_socket);
        FD_CLR(client_socket, master);
    } 
    else {
        Command *command = parse_input(buf);
        handle_user(command, client, logged_users);
        free_command(command);
    }

    free_user(client);
}

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

void run_server(int listening_socket, volatile sig_atomic_t *server_running) {
    fd_set master;
    FD_ZERO(&master);
    fd_set read_fds;
    FD_ZERO(&read_fds);

    if (listen(listening_socket, BACKLOG) == -1) {
        perror("[SERVER] listen");
        close(listening_socket);
        exit(EXIT_FAILURE);
    }

    FD_SET(listening_socket, &master);
    int max_fd = listening_socket; 

    User *logged_users[HASHTABLE_SIZE];
    init_hash_table(logged_users);
    log_event("INFO", "Server initiated.");
    
    while (*server_running) {
        read_fds = master;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("[SERVER] select");
            break;
        }

        for (int i = 0; i <= max_fd; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listening_socket) { 
                    handle_new_connections(listening_socket, &master, &max_fd);
                }
                else {
                    handle_client_data(i, &master, logged_users);
                }
            }
        }
    }

    for (int i = 0; i <= max_fd; ++i) {
        if (FD_ISSET(i, &master)) close(i);
    }

    clear_hash_table(logged_users);
    log_event("INFO", "Server terminated.");
}