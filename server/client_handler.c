#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "hashing.h"
#include "command_parser.h"
#include "message.h"

void login_user(Command *cmd, int client_socket, User *logged_users[]) {
    User *user = create_user(cmd->arg2, client_socket);
    insert_user(logged_users, user);

    char message[1024];
    snprintf(message, sizeof message, "User '%s' logged in from socket %d.", user->username, user->socket);
    log_event("LOGIN", message);
}