#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "command.h"

struct command {
    char *first;
    char *second;
    char *third;
};

Command *create_command(const char *first, const char *second, const char *third) {
    Command *command = malloc(sizeof(Command));
    if (!command) {
        perror("[SERVER] Failed to allocate command");
        return NULL;
    }

    command->first = strdup(first);
    if (!command->first) {
        perror("[SERVER] Failed to copy first token");
        
        free(command);
        return NULL;
    }

    if (second) {
        command->second = strdup(second);
        if (!command->second) {
            perror("[SERVER] Failed to copy second token");

            free(command->first);
            free(command);
            return NULL;
        }
    }
    else {
        command->second = NULL;
    }

    if (third) {
        command->third = strdup(third);
        if (!command->third) {
            perror("[SERVER] Failed to copy third token");
            
            free(command->first);
            if (command->second) free(command->second);
            free(command);
            return NULL;
        }
    }
    else {
        command->third = NULL;
    }

    return command;
}

void free_command(Command *command) {
    if (!command) {
        return;
    }

    free(command->first);
    if (command->second) free(command->second);
    if (command->third) free(command->third);
    
    free(command);
}

const char *get_first_token(const Command *command) {
    return command ? command->first : NULL;
}

const char *get_second_token(const Command *command) {
    return command ? command->second : NULL;
}

const char *get_third_token(const Command *command) {
    return command ? command->third : NULL;
}