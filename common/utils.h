#ifndef UTILS_H
#define UTILS_H
          
#include <arpa/inet.h>  

#include "user_handler.h"

void *get_in_addr(struct sockaddr *sa);
int send_message(User *client, const char *message);
int receive_message(User *client, char *buffer, size_t buffer_size);

#endif