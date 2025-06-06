#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      // Provides access to the POSIX API — includes close(), read(), write(), fork(), etc.
#include <errno.h>       // Defines the errno variable and error codes.
#include <string.h>      
#include <sys/types.h>   // Defines data types used in system calls like pid_t, size_t, ssize_t.
#include <sys/socket.h>  // Provides socket-related functions and constants: socket(), bind(), listen(), accept(), etc.
#include <netinet/in.h>  // Defines structures for internet domain addresses, like struct sockaddr_in.
#include <netdb.h>       // Used for DNS lookups and hostname translation (getaddrinfo, gethostbyname, etc).
#include <arpa/inet.h>   // Functions for IP address conversion, like inet_ntop, inet_pton.

void get_listening_socket(int *sockfd);
void get_connecting_sockets(int sockfd, fd_set *master, int *fdmax);

#endif