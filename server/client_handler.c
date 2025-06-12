#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "hashing.h"
#include "command_parser.h"
#include "message.h"

void login_user(Command *cmd, User *client, User *logged_users[]);
void logout_user(User *client, User *logged_users[]);
void private_message(Command* cmd, User *sender, User *logged_users[]);
void list_logged_user(User* client, User *logged_users[]);

void handle_client(Command *cmd, User *client, User *logged_users[]) {
    char *tag = cmd->arg1;
    
    if (strcmp(tag, "/login") == 0) login_user(cmd, client, logged_users);
    else if (strcmp(tag, "/logout") == 0) logout_user(client, logged_users);
    else if (strcmp(tag, "/list") == 0) list_logged_user(client, logged_users);
    else if (strcmp(tag, "/send") == 0) private_message(cmd, client, logged_users); 
}

void login_user(Command *cmd, User *client, User *logged_users[]) {
    User *user = create_user(strdup(cmd->arg2), client->socket);
    insert_user(logged_users, user);

    char message[1024];
    snprintf(message, sizeof message, "User '%s' logged in from socket %d.", user->username, user->socket);
    log_event("LOGIN", message);
}

void logout_user(User *client, User *logged_users[]) {
    User *temp = create_user("", client->socket);

    int deleted;
    if ((deleted = delete_user(logged_users, client)) == 0) {
        client = temp;
        send_message(client, "Logout failed: You were not logged in.");
        return;
    }
    client = temp;
    send_message(client, "You have been successfully logged out. See you soon!");
}

void private_message(Command *cmd, User *sender, User *logged_users[]) {
    char *username = strdup(cmd->arg2);
    char *message = strdup(cmd->arg3);

    User *receiver = find_username(logged_users, username);
    if (!receiver) {
        char message[1024];
        snprintf(message, sizeof message, "User '%s' is not connected to server.", username);
        send_message(sender, message);

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
    snprintf(encapsulated_message, sizeof encapsulated_message, "[%s] %s -> %s", timestamp, sender->username, message);
    send_message(receiver, encapsulated_message);
}

void list_logged_user(User *client, User *logged_users[]) {
    char message[2048];
    message[0] = '\0';
    
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = logged_users[i];

        while (current) {
            if (current->username == NULL) { 
                current = current->next;
                continue;
            }

            if (strlen(message) + strlen(current->username) + 2 >= sizeof(message)) {
                break;
            }

            strcat(message, current->username);
            strcat(message, "\n");

            current = current -> next;
        }
    }

    if (strlen(message) == 0) {
        send_message(client, "No users are currently logged in.\n");
        return;
    }

    send_message(client, message);
}