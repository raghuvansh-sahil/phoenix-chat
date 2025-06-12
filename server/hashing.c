#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user_handler.h"

#define HASHTABLE_SIZE 101

void init_hash_table(User *hash_table[]) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        hash_table[i] = NULL;
    }
}

unsigned long hash_function(const char *username) {
    unsigned long hash = 5381;
    int c;

    while ((c = *username++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash % HASHTABLE_SIZE;
}

void insert_user(User *hash_table[], User *user) {
    unsigned long index = hash_function(user->username);

    user->next = hash_table[index];
    hash_table[index] = user;
}

int delete_user(User *hash_table[], User *user) {
    unsigned long index = hash_function(user->username);
    
    User *previous = NULL;
    User *current = hash_table[index];
    while (current) {
        if (strcmp(current->username, user->username) == 0) {
            if (previous == NULL) {
                hash_table[index] = current->next;
            }
            else {
                previous->next = current->next;
            }
            current->next = NULL;

            free_user(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    return 0;
}

User *find_username(User *hash_table[], const char *username) {
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

User *find_socket(User *hash_table[], int socket) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = hash_table[i];

        while (current) {
            if (current->socket == socket) {
                return current;
            }
            current = current->next;
        }
    }

    return NULL;
}

void clear_hash_table(User *hash_table[]) {
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = hash_table[i];
        while (current) {
            User *temp = current;
            current = current->next;
            free_user(temp);
        }
    }
}