#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "utils.h"

#define PORT "21111"
#define IPADDRESS "192.168.31.129"

void connect_to_server(int *client_socket) {
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(IPADDRESS, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "[CLIENT] getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*client_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("[CLIENT] socket");
            continue;
        }

        if (connect(*client_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(*client_socket);
            perror("[CLIENT] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "[CLIENT] Failed to connect.\n");
        exit(EXIT_FAILURE);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    
    freeaddrinfo(servinfo); 

    char buf[1024];
    receive_message(*client_socket, buf, sizeof buf); 
    printf("%s", buf);
}

void look_for_data(int client_socket) {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    FD_SET(client_socket, &read_fds);

    int activity = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);
    if (activity == -1) {
        perror("[CLIENT] select");
        return;
    } 
    else if (activity == 0) {
        return;
    }

    if (FD_ISSET(client_socket, &read_fds)) {
        char buf[2048];
        int bytesReceived = receive_message(client_socket, buf, sizeof buf);
        
        if (bytesReceived > 0) {
            printf("%s\n", buf);
        } 
        else if (bytesReceived == 0) {
            printf("[INFO] Server closed connection.\n");
        } 
    }
}