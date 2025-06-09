#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "client.h"
#include "user_handler.h"

int main(void) {
    char buf[2048] = "";
    int client_socket;

    User *client = connect_to_server(&client_socket);
    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "/exit") == 0)
            break;

        send_message(client, buf);
        look_for_data(client);
    }
    close(client_socket);
    free_user(client);
    
    return 0;
}