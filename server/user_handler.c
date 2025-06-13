#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "hashing.h"
#include "message.h"
#include "user_handler.h"

static void login_user(const Command *command, User *user, User *logged_users[]) {
    const char *username = get_second_token(command);
    int socket = get_socket(user);

    if (!username) {
        const char *msg = "Invalid login format. Usage: /login <username>";
        send_message(user, msg, strlen(msg) + 1);
        return;
    }

    if (find_user_by_username(logged_users, username)) {
        const char *message = "Login failed: User already logged in.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    insert_user(logged_users, create_user_with_username(username, socket));
    const char *message = "Login successful. Welcome!";
    send_message(user, message, strlen(message) + 1);

    char log[1024];
    snprintf(log, sizeof log, "User '%s' logged in.", username);
    log_event("LOGIN", log);
}

static void logout_user(User *user, User *logged_users[]) {
    User *to_be_deleted = delete_user(logged_users, user);

    if (!to_be_deleted) {
        const char *message = "Logout failed: You are not logged in.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    const char *message = "You have been successfully logged out. See you soon!";
    send_message(user, message, strlen(message) + 1);

    char log[1024];
    snprintf(log, sizeof log, "User '%s' logged out.", get_username(to_be_deleted));
    log_event("LOGOUT", log);

    free_user(to_be_deleted);
}

static void list_logged_users(User *user, User *logged_users[]) {
    size_t list_size = 128;
    size_t used = 0;

    char *list = malloc(list_size);
    if (!list) {
        perror("[SERVER] Failed to allocate list");
        return;
    }
    list[0] = '\0';
    
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = logged_users[i];

        while (current) {
            const char* username = get_username(current);
            size_t length = strlen(username) + 1;

            if (used + length + 1 > list_size) {
                while (used + length + 1 > list_size) {
                    list_size *= 2;
                }

                char *new_list = realloc(list, list_size);
                if (!new_list) {
                    perror("[SERVER] Failed to reallocate list");
                    free(list);
                    return;
                }
                list = new_list;
            }

            strcat(list, username);
            used += strlen(username);

            strcat(list, "\n");
            used += 1;

            current = get_next_user(current);
        }
    }

    if (used == 0) {
        const char* message =  "No users are currently logged in.";
        send_message(user, message, strlen(message) + 1);
        free(list);
        return;
    }

    list[used - 1] = '\0';
    send_message(user, list, strlen(list) + 1);
    free(list);
}

static void private_message(const Command *command, User *user, User *logged_users[]) {
    int socket = get_socket(user);
    User *sender = find_user_by_socket(logged_users, socket);

    const char *receiver_username = get_second_token(command);
    const char *message_content = get_third_token(command);
    if (!receiver_username || !message_content) {
        const char *message = "Invalid private message format. Usage: /send <username> <message>";
        send_message(sender, message, strlen(message) + 1);

        return;
    }

    User *receiver = find_user_by_username(logged_users, receiver_username);
    if (!receiver) {
        char message[1024];
        snprintf(message, sizeof message, "User '%s' is not connected to server.", receiver_username);
        send_message(sender, message, strlen(message) + 1);

        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (tm_info == NULL) {
        perror("[SERVER] Failed to get local time");
        return;
    }

    char timestamp[6];
    strftime(timestamp, 6, "%H:%M", tm_info);

    char message[1024];
    snprintf(message, sizeof message, "[DM from %s @ %s]: %s", get_username(sender), timestamp, message_content);
    send_message(receiver, message, strlen(message) + 1);
}

void handle_user(const Command *command, User *client, User *logged_users[]) {
    const char *tag = get_first_token(command);
    
    if (strcmp(tag, "/login") == 0) {
        login_user(command, client, logged_users);
    }
    else if (strcmp(tag, "/logout") == 0) {
        logout_user(client, logged_users);
    }
    else if (strcmp(tag, "/list") == 0) {
        list_logged_users(client, logged_users);
    } 
    else if (strcmp(tag, "/send") == 0) {
        private_message(command, client, logged_users); 
    }
}