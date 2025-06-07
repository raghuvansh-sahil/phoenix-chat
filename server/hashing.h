#ifndef HASHING_H
#define HASHING_H

#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 64
#define HASHTABLE_SIZE 101

typedef struct user {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct user *next;
} User;


void init_hash_table(User *hash_table[]);
void insert_user(User *hash_table[], const char *username, const char *password);
int load_all_users(User *hash_table[]);
User *find_user(User *hash_table[], const char *username);
void clear_hash_table(User *hash_table[]);

#endif