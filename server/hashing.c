#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashing.h"

static unsigned long hash_username(const char *username) {
    unsigned long hash = 5381;
    int c;

    while ((c = *username++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash % HASHTABLE_SIZE;
}

void init_hash_table(User *hash_table[]) {
    if (!hash_table) {
        return;
    }

    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        hash_table[i] = NULL;
    }
}

void insert_user(User *hash_table[], User *user) {
    if (!hash_table || !user) {
        return;
    }

    const char *username = get_username(user);
    unsigned long index = hash_username(username);
    
    set_next_user(user, hash_table[index]);
    hash_table[index] = user;
}

User *find_user_by_username(User *hash_table[], const char *username) {
    if (!hash_table || !username) {
        return NULL;
    }

    unsigned long index = hash_username(username);
    
    User *current = hash_table[index];
    while (current) {
        if (strcmp(get_username(current), username) == 0) {
            return current;
        }
        current = get_next_user(current);
    }

    return NULL;
}

User *find_user_by_socket(User *hash_table[], int socket) {
    if (!hash_table) {
        return NULL;
    }

    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = hash_table[i];

        while (current) {
            if (get_socket(current) == socket) {
                return current;
            }
            current = get_next_user(current);
        }
    }

    return NULL;
}

void clear_hash_table(User *hash_table[]) {
    if (!hash_table) {
        return;
    }

    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        User *current = hash_table[i];

        while (current) {
            User *temp = current;
            current = get_next_user(current);
            free_user(temp);
        }

        hash_table[i] = NULL;
    }
}