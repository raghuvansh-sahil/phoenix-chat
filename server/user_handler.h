#ifndef USER_HANDLER_H
#define USER_HANDLER_H

#include "user.h"
#include "command.h"

void handle_user(const Command *command, User* user, User *logged_users[]);

#endif