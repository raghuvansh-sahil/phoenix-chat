#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user {
    char *username;
    int socket;
    struct user *next;
} User;

User *create_user(const char* username, const int socket) {
    User* user = (User *)malloc(sizeof(User));
    if (!user) {
        perror("[SERVER] failed to allocate user");
        return NULL;
    }

    user->username = strdup(username);
    if (!user->username) {
        perror("[SERVER] Failed to copy username");
        free(user);
        return NULL;
    }

    user->socket = socket;
    user->next = NULL;

    return user;
}

void free_user(User *user) {
    if (!user) {
        return;
    }

    free(user->username);
    free(user);
}