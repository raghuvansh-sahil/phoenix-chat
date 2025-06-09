#ifndef USER_HANDLER_H
#define USER_HANDLER_H

typedef struct user {
    char *username;
    int socket;
    struct user *next;
} User;

User *create_user(const char* username, const int socket);
void free_user(User *user);

#endif