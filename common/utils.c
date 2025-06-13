#include <stdio.h>       
#include <stdlib.h>  
#include <errno.h>       
#include <unistd.h>      
#include <string.h>  


#include "utils.h"

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

ssize_t send_message(User *user, const char *message, size_t message_size) {
    ssize_t total = 0;
    size_t bytes_left = message_size;
    int socket = get_socket(user);
    
    ssize_t n;
    while (total < message_size) {
        n = send(socket, message + total, bytes_left, 0);
        if (n == -1) {
            perror("send");
            return -1;
        }
        total += n;
        bytes_left -= n;
    }
    
    return total;
}

ssize_t receive_message(User *user, char *buffer, size_t buffer_size) {
    ssize_t bytesReceived;
    int socket = get_socket(user);

    if ((bytesReceived = recv(socket, buffer, buffer_size - 1, 0)) == -1) {
        perror("recv");
        return -1;
    }
    buffer[bytesReceived] = '\0'; 

    return bytesReceived;
}