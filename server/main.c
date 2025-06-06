#include "server.h"

int main() {
    int sockfd, new_fd;

    // Get a listening socket
    get_listening_socket(&sockfd);

    // Accept incoming connections
    while (1) {
        get_connecting_sockets(sockfd, &new_fd);
        // Here you would typically handle the new connection, e.g., create a thread or process to manage it.
        close(new_fd);  // Close the connection after handling it for simplicity.
    }

    close(sockfd);  // Close the listening socket when done.
    return 0;
}