#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct command {
    char *arg1;
    char *agr2;
    char *agr3;
} Command;

Command *create_command(const char *arg1, const char *arg2, const char *arg3) {
    Command *command = (Command *)malloc(sizeof(Command));
    if (!command) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    command->arg1 = arg1 ? strdup(arg1) : NULL;
    command->agr2 = arg2 ? strdup(arg2) : NULL;
    command->agr3 = arg3 ? strdup(arg3) : NULL;

    return command;
}


Command *parse_command(char *input) {
    printf("Parsing input: %s\n", input);
    Command *command = create_command(NULL, NULL, NULL);

    char *arg = strtok(input, " ");
    for (int i = 0; arg != NULL; ++i) {
        switch(i) {
        case 0:
            command->arg1 = strdup(arg);
            break;
        case 1:
            command->agr2 = strdup(arg);
            break;
        case 2:  
            command->agr3 = strdup(arg);
            break;  
        }
    }

    return command;
}

int main() {
    return 0;
}