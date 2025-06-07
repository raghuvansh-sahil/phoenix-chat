#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 32
#define MAX_PASSWORD_LENGTH 64
#define HASHTABLE_SIZE 101

typedef struct user {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct user *next;
} User;

unsigned long hash_function(const char *username) {
    unsigned long hash = 5381;
    int c;

    while ((c = *username++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash % HASHTABLE_SIZE;
}

void init_hash_table(User *hash_table[]) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        hash_table[i] = NULL;
    }
}

void insert_user(User *hash_table[], const char *username, const char *password) {
    unsigned long index = hash_function(username);
    User *new_user = malloc(sizeof(User));
    if (!new_user) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(new_user->username, username, MAX_USERNAME_LENGTH);
    strncpy(new_user->password, password, MAX_PASSWORD_LENGTH);

    new_user->next = hash_table[index];
    hash_table[index] = new_user;
}

int load_all_users(User *hash_table[]) {
    FILE *file = fopen("users.db", "r");
    if (!file) {
        perror("Failed to open users file");
        return -1;
    }

    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    while (fscanf(file, "%s %s", username, password) == 2) {
        insert_user(hash_table, username, password);
    }

    fclose(file);
    return 0;
}

User *find_user(User *hash_table[], const char *username) {
    unsigned long index = hash_function(username);
    User *current = hash_table[index];

    while (current) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void clear_hash_table(User *hash_table[]) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = hash_table[i];
        while (current) {
            User *temp = current;
            current = current->next;
            free(temp);
        }
    }
}