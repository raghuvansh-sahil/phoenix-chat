#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct command {
    char *arg1;
    char *arg2;
    char *arg3;
} Command;

Command *parse_input(char *input) {
    Command *cmd = malloc(sizeof(Command));
    if (!cmd) {
        perror("[SERVER] failed to allocate command");
        return NULL;
    }

    char *copy = strdup(input);
    if (!copy) {
        perror("[SERVER] failed to duplicate command");
        free(cmd);
        return NULL;
    }

    char *arg1 = strtok(copy, " ");
    if (arg1) cmd->arg1 = strdup(arg1); else cmd->arg1 = NULL;

    char *arg2 = strtok(NULL, " ");
    if (arg2) cmd->arg2 = strdup(arg2); else cmd->arg2 = NULL;

    if (cmd->arg1 && cmd->arg2) {
        char *arg3 = input;

        arg3 += strlen(cmd->arg1);
        while (*arg3 == ' ') arg3++; 

        arg3 += strlen(cmd->arg2);
        while (*arg3 == ' ') arg3++;

        if (*arg3) cmd->arg3 = strdup(arg3); else cmd->arg3 = NULL;
    } 
    else {
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