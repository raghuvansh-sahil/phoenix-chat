#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "user_handler.h"
#include "hashing.h"
#include "command_parser.h"

void handle_client(Command *cmd, User* client, User *logged_users[]);

#endif