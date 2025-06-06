#ifndef UTILS_H
#define UTILS_H
          
#include <arpa/inet.h>   

void *get_in_addr(struct sockaddr *sa);
int send_message(int sockfd, const char *message);
int receive_message(int sockfd, char *buffer, size_t buffer_size);

#endif