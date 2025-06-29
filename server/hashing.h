#ifndef HASHING_H
#define HASHING_H

#include "user.h"

#define HASHTABLE_SIZE 101

void init_hash_table(User *hash_table[]);

void insert_user(User *hash_table[], User *user);

User *find_user_by_username(User *hash_table[], const char *username);
User *find_user_by_socket(User *hash_table[], int socket);

void clear_hash_table(User *hash_table[]);

#endif