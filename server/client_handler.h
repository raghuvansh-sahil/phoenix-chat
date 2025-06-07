#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "hashing.h"
#include "command_parser.h"

void login_user(Command *cmd, int client_socket, User *logged_users[]);

#endif