#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      
#include <string.h>      
#include <arpa/inet.h>   

#include "user_handler.h"

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int send_message(User *client, const char *message) {
    int bytesSent;

    if ((bytesSent = send(client->socket, message, strlen(message), 0)) == -1) {
        perror("send");
        close(client->socket);
        exit(EXIT_FAILURE);
    }

    return bytesSent;
}

int receive_message(User *client, char *buffer, size_t buffer_size) {
    int bytesReceived;

    if ((bytesReceived = recv(client->socket, buffer, buffer_size - 1, 0)) == -1) {
        perror("recv");
        close(client->socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytesReceived] = '\0'; 

    return bytesReceived;
}