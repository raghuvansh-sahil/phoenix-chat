#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "user.h"
#include "utils.h"
#include "client.h"

int main(void) {
    char buf[1024] = "";
    int server_socket;

    User *client = connect_to_server(&server_socket);
    if (!client) {
        exit(EXIT_FAILURE);
    }
    while (1) {
        printf(">> ");
        //fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        } buf[strcspn(buf, "\n")] = '\0';

        send_message(client, buf, strlen(buf) + 1);

        poll_server_messages(client);
        if (strcmp(buf, "/logout") == 0) {
            break;
        }
    }
    close(server_socket);
    free_user(client);
    
    return 0;
}