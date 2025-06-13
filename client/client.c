#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"
#include "client.h"

#define PORT "21111"
#define IPADDRESS "192.168.31.129"

User *connect_to_server(int *server_socket) {
    struct addrinfo hints, *servinfo, *p;
    int rv;
    
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(IPADDRESS, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "[CLIENT] getaddrinfo: %s\n", gai_strerror(rv));
        return NULL;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("[CLIENT] socket");
            continue;
        }

        if (connect(*server_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(*server_socket);
            perror("[CLIENT] connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        freeaddrinfo(servinfo);
        fprintf(stderr, "[CLIENT] Failed to connect.\n");
        return NULL;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    
    freeaddrinfo(servinfo); 

    User *client = create_user_with_socket(*server_socket);

    char buf[1024];
    int bytesReceived = receive_message(client, buf, sizeof buf);
    if (bytesReceived > 0) {
        printf("%s\n", buf);
        return client;
    }
    else {
        fprintf(stderr, "[CLIENT] Failed to receive greeting from server.\n");
        free_user(client);
        return NULL;
    }
}

void poll_server_messages(User *client) {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int socket = get_socket(client);

    FD_SET(socket, &read_fds);

    int activity = select(socket + 1, &read_fds, NULL, NULL, &timeout);
    if (activity == -1) {
        perror("[CLIENT] select");
        return;
    } 
    else if (activity == 0) {
        return;
    }

    if (FD_ISSET(socket, &read_fds)) {
        char buf[1048];
        int bytesReceived = receive_message(client, buf, sizeof buf);
        
        if (bytesReceived > 0) {
            printf("%s\n", buf);
        } 
        else if (bytesReceived == 0) {
            printf("[INFO] Server closed connection.\n");
        } 
    }
}