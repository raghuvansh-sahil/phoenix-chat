#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>       

#include "hashing.h"
#include "user_handler.h"

void get_listening_socket(int *listening_socket);
void get_connecting_sockets(int listening_socket, fd_set *master, int *max_fd, User *logged_users[]);

#endif