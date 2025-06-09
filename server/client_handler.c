#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "hashing.h"
#include "command_parser.h"
#include "message.h"

void login_user(Command *cmd, int client_socket, User *logged_users[]);
void private_message(Command *cmd, int client_socket, User *logged_users[]);

void handle_client(Command *cmd, int client_socket, User *logged_users[]) {
    char *tag = cmd->arg1;
    
    if (strcmp(tag, "/login") == 0) login_user(cmd, client_socket, logged_users);
    else if (strcmp(tag, "/send") == 0) private_message(cmd, client_socket, logged_users); 
}

void login_user(Command *cmd, int client_socket, User *logged_users[]) {
    User *user = create_user(cmd->arg2, client_socket);
    insert_user(logged_users, user);

    char message[1024];
    snprintf(message, sizeof message, "User '%s' logged in from socket %d.", user->username, user->socket);
    log_event("LOGIN", message);
}

void private_message(Command *cmd, int client_socket, User *logged_users[]) {
    char *username = strdup(cmd->arg2);
    char *message = strdup(cmd->arg3);

    User *receiver = find_user(logged_users, username);
    if (!receiver) {
        char message[1024];
        snprintf(message, sizeof message, "User '%s' is not connected to server.", username);
        send_message(client_socket, message);

        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (tm_info == NULL) {
        perror("Failed to get local time");
        return;
    }

    char timestamp[20];
    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    char encapsulated_message[2048];
    snprintf(encapsulated_message, sizeof encapsulated_message, "[%s] %d -> %s\n", timestamp, client_socket, message);
    send_message(receiver->socket, encapsulated_message);
}