#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handler.h"
#include "hashing.h"

void fetch_users_from_database(User *users[]) {
    FILE *db = fopen("server/users.db", "r");
    if (!db) {
        perror("[SERVER] Failed to open database");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof line, db)) {
        char *username = strtok(line, " ");
        char *password = strtok(NULL, "\n");

        if (username && password) {
            User *user = create_user_with_credentials(username, password);
            insert_user(users, user);
        }
    }

    fclose(db);
}