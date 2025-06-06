#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      
#include <string.h>      
#include <arpa/inet.h>   

// get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// send message
int send_message(int sockfd, const char *message) {
    int bytesSent;

    if ((bytesSent = send(sockfd, message, strlen(message), 0)) == -1) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return bytesSent;
}

// receive message
int receive_message(int sockfd, char *buffer, size_t buffer_size) {
    int bytesReceived;

    if ((bytesReceived = recv(sockfd, buffer, buffer_size - 1, 0)) == -1) {
        perror("recv");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[bytesReceived] = '\0'; 

    return bytesReceived;
}