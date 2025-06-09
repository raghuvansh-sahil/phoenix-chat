#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "client.h"

int main(void) {
    char buf[2048] = "";
    int clientSocket;

    connect_to_server(&clientSocket);

    while (1) {
        printf("> ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strcmp(buf, "/exit") == 0)
            break;

        send_message(clientSocket, buf);
        look_for_data(clientSocket);
    }

    close(clientSocket);
    return 0;
}