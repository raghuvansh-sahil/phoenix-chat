#ifndef USER_H
#define USER_H

#include <stdbool.h>

typedef struct user User;

User *create_user_with_socket(const int socket);
User *create_user_with_username(const char* username, const int socket);
User *create_user_with_credentials(const char* username, const char *password);

void free_user(User *user);

const char *get_username(const User *user);
const char *get_password(const User *user);
int get_socket(const User *user);

User *get_next_user(const User *user);
void set_next_user(User *user, User *next);

#endif