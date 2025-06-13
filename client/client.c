#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "utils.h"
#include "user.h"

#define PORT "21111"
#define IPADDRESS "192.168.31.129"

User *connect_to_server(int *client_socket) {
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

    User *client = create_user("", *client_socket);

    char buf[1024];
    receive_message(client, buf, sizeof buf); 
    printf("%s\n", buf);

    return client;
}

void look_for_data(User *client) {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    FD_SET(client->socket, &read_fds);

    int activity = select(client->socket + 1, &read_fds, NULL, NULL, &timeout);
    if (activity == -1) {
        perror("[CLIENT] select");
        return;
    } 
    else if (activity == 0) {
        return;
    }

    if (FD_ISSET(client->socket, &read_fds)) {
        char buf[2048];
        int bytesReceived = receive_message(client, buf, sizeof buf);
        
        if (bytesReceived > 0) {
            printf("%s\n", buf);
        } 
        else if (bytesReceived == 0) {
            printf("[INFO] Server closed connection.\n");
        } 
    }
}