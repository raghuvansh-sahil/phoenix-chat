#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "hashing.h"
#include "message.h"
#include "bcrypt.h"
#include "user_handler.h"

static void login_user(const Command *command, User *user, User *users[]) {
    const char *username = get_second_token(command);
    int socket = get_socket(user);

    User *temp = find_user_by_socket(users, socket);
    if (temp) {
        const char *message = "You are already logged in.";
        send_message(user, message, strlen(message) + 1);
        return;
    }

    if (!username) {
        const char *message = "Invalid login format. Usage: /login <username>.";
        send_message(user, message, strlen(message) + 1);
        return;
    }

    char password[61];
    User *logging_in = find_user_by_username(users, username);
    if (!logging_in) {
        const char *message = "Login failed: User not registered.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    if (is_logged_in(logging_in)) {
        const char *message = "Login failed: This user is already logged in elsewhere.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
        
    const char *message = "Enter password:";
    send_message(user, message, strlen(message) + 1);

    ssize_t bytes_received = receive_message(user, password, sizeof password);
    if (bytes_received <= 0) {
        const char *message = "Failed to receive password.";
        send_message(user, message, strlen(message) + 1);
        return;
    }

    int matched = bcrypt_checkpw(password, get_password(logging_in));
    if (matched == -1) {
        perror("[SERVER] bcrypt_checkpw");

        const char *message = "Server error during login.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    else if (matched > 0) {
        const char *message = "Login failed: Incorrect password entered!";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    else if (matched == 0) {
        toggle_login_status(logging_in);
        set_socket(logging_in, socket);

        const char *message = "Login successful. Welcome!";
        send_message(user, message, strlen(message) + 1);
    }

    char log[1024];
    snprintf(log, sizeof log, "User '%s' logged in.", username);
    log_event("LOGIN", log);
}

static void logout_user(User *user, User *users[]) {
    int socket = get_socket(user);
    User *logging_out = find_user_by_socket(users, socket);

    if (!is_logged_in(logging_out)) {
        const char *message = "Logout failed: You are not logged in.";
        send_message(user, message, strlen(message) + 1);
        return;
    }
    const char *message = "You have been successfully logged out. See you soon!";
    send_message(user, message, strlen(message) + 1);

    char log[1024];
    snprintf(log, sizeof log, "User '%s' logged out.", get_username(logging_out));
    log_event("LOGOUT", log);

    toggle_login_status(logging_out);
    set_socket(logging_out, -1);
}

static void list_logged_users(User *user, User *users[]) {
    size_t list_size = 128;
    size_t used = 0;

    char *list = malloc(list_size);
    if (!list) {
        perror("[SERVER] Failed to allocate list");
        return;
    }
    list[0] = '\0';
    
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = users[i];

        while (current) {
            if (!is_logged_in(current)) {
                current = get_next_user(current);
                continue;
            }

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

static void private_message(const Command *command, User *user, User *users[]) {
    int socket = get_socket(user);
    User *sender = find_user_by_socket(users, socket);

    const char *receiver_username = get_second_token(command);
    const char *message_content = get_third_token(command);
    if (!receiver_username || !message_content) {
        const char *message = "Invalid private message format. Usage: /send <username> <message>";
        send_message(sender, message, strlen(message) + 1);

        return;
    }

    User *receiver = find_user_by_username(users, receiver_username);
    if (get_socket(receiver) == -1) {
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

void handle_user(const Command *command, User *client, User *users[]) {
    const char *tag = get_first_token(command);
    
    if (strcmp(tag, "/login") == 0) {
        login_user(command, client, users);
    }
    else if (strcmp(tag, "/logout") == 0) {
        logout_user(client, users);
    }
    else if (strcmp(tag, "/list") == 0) {
        list_logged_users(client, users);
    } 
    else if (strcmp(tag, "/send") == 0) {
        private_message(command, client, users); 
    }
}