#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

typedef struct command {
    char *arg1;
    char *agr2;
    char *agr3;
} Command;

Command *parse_command(char *input);

#endif 