#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

typedef struct command {
    char *arg1;
    char *arg2;
    char *arg3;
} Command;

Command *parse_input(const char *input);
void free_command(Command *cmd);

#endif 