#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

User *connect_to_server(int *client_socket);
void poll_server_messages(User *client);

#endif