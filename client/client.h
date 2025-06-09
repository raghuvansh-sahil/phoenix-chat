#ifndef CLIENT_H
#define CLIENT_H

void connect_to_server(int *client_socket);
void look_for_data(int client_socket);

#endif