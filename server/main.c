#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"
#include "user_handler.h"
#include "hashing.h"

int main() {
    int listening_socket;
    get_listening_socket(&listening_socket);

    fd_set master;
    FD_ZERO(&master);
    int max_fd;

    User *logged_users[HASHTABLE_SIZE];

    init_hash_table(logged_users);
    while (1) {
        get_connecting_sockets(listening_socket, &master, &max_fd, logged_users);
    }
    close(listening_socket); 
    clear_hash_table(logged_users);

    return 0;
}