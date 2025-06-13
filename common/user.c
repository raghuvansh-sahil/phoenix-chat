#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

#include "user.h"

struct user {
    char *username;
    char *password;
    int socket;
    bool logged_in;
    struct user *next;
};

User *create_user_with_socket(const int socket) {
    User* user = (User *)malloc(sizeof(User));
    if (!user) {
        perror("[SERVER] Failed to allocate user");
        return NULL;
    }

    user->username = NULL;
    user->password = NULL;
    user->socket = socket;
    user->logged_in = false;
    user->next = NULL;

    return user;
}

User *create_user_with_credentials(const char* username, const char *password) {
    User* user = (User *)malloc(sizeof(User));
    if (!user) {
        perror("[SERVER] Failed to allocate user");

        return NULL;
    }

    user->username = strdup(username);
    if (!user->username) {
        perror("[SERVER] Failed to copy username");

        free(user);
        return NULL;
    }

    user->password = strdup(password);
    if (!user->password) {
        perror("[SERVER] Failed to copy password");

        free(user->username);
        free(user);
        return NULL;
    }

    user->socket = -1;  // Default invalid socket
    user->logged_in = false;
    user->next = NULL;

    return user;
}

void free_user(User *user) {
    if (!user) {
        return;
    }

    if (user->username) free(user->username);
    if (user->password) free(user->password);

    free(user);
}

const char *get_username(const User *user) {
    return user ? user->username : NULL;
}

const char *get_password(const User *user) {
    return user ? user->password : NULL;
}

int get_socket(const User *user) {
    return user ? user->socket : -1;
}

void set_socket(User *user, int socket) {
    user->socket = socket;
}

bool is_logged_in(const User *user) {
    return user ? user->logged_in : false;
}

void toggle_login_status(User *user) {
    user->logged_in = !user->logged_in;
}

User *get_next_user(const User *user) {
    return user ? user->next : NULL;
}

void set_next_user(User *user, User *next) {
    user->next = next;
}