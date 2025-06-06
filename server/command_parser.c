#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *arg1;
    char *arg2;
    char *arg3;
} Command;

Command *parse_command(const char *input) {
    Command *cmd = malloc(sizeof(Command));
    if (!cmd) {
        perror("malloc");
        return NULL;
    }

    // Make a mutable copy of input
    char *copy = strdup(input);
    if (!copy) {
        perror("strdup");
        free(cmd);
        return NULL;
    }

    char *token = strtok(copy, " ");
    if (token) cmd->arg1 = strdup(token); else cmd->arg1 = NULL;

    token = strtok(NULL, " ");
    if (token) cmd->arg2 = strdup(token); else cmd->arg2 = NULL;

    // Get the position where arg3 starts
    if (cmd->arg1 && cmd->arg2) {
        // Find where the third argument starts in the original input
        const char *third_start = input;

        // Skip over arg1
        third_start += strlen(cmd->arg1);
        while (*third_start == ' ') third_start++; // skip spaces

        // Skip over arg2
        third_start += strlen(cmd->arg2);
        while (*third_start == ' ') third_start++; // skip spaces

        // Now third_start points to the start of arg3
        if (*third_start)
            cmd->arg3 = strdup(third_start);
        else
            cmd->arg3 = NULL;
    } else {
        cmd->arg3 = NULL;
    }

    free(copy);
    return cmd;
}

void free_command(Command *cmd) {
    free(cmd->arg1);
    free(cmd->arg2);
    free(cmd->arg3);
    free(cmd);
}