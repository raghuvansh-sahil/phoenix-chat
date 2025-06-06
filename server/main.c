#include "server.h"

int main() {
    fd_set master;
    int fdmax;

    int sockfd;

    // Get a listening socket
    get_listening_socket(&sockfd);

    // Accept incoming connections
    while (1) {
        get_connecting_sockets(sockfd, &master, &fdmax);
    }

    close(sockfd);  // Close the listening socket when done.
    return 0;
}