#ifndef CLIENT_H
#define CLIENT_H

#include "user_handler.h"

User *connect_to_server(int *client_socket);
void look_for_data(User *client);

#endif