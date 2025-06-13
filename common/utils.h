#ifndef UTILS_H
#define UTILS_H
          
#include <arpa/inet.h>  

#include "user.h"

void *get_in_addr(struct sockaddr *sa);

ssize_t send_message(User *user, const char *message, size_t message_size);
ssize_t receive_message(User *user, char *buffer, size_t buffer_size);

#endif