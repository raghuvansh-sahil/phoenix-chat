#include "client.h"

int main(void) {
    int sockfd;

    // Connect to the server
    connect_to_server(&sockfd);

    // Here you can add code to send/receive messages using sockfd

    // Close the socket when done
    close(sockfd);
    
    return 0;
}