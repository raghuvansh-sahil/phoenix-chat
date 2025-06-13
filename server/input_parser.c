#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_parser.h"

Command *parse_input(const char *input) {
    if (!input || strlen(input) == 0) {
        return NULL;
    }

    char *copy = strdup(input);
    if (!copy) {         
        perror("[SERVER] Failed to copy command");
        return NULL;
    }

    char *first = strtok(copy, " ");

    char *second = strtok(NULL, " ");

    const char *third = NULL;
    if (first && second) {
        third = input;

        third += strlen(first);
        while (*third == ' ') third++; 

        third += strlen(second);
        while (*third == ' ') third++;

        if (*third == '\0') {
            third = NULL;
        }
    } 

    Command *command = create_command(first, second, third);
    free(copy);
    return command;
}