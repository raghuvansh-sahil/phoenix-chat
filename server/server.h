#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>  
#include <signal.h>     

void get_listening_socket(int *listening_socket);
void run_server(int listening_socket, volatile sig_atomic_t *server_running);

#endif