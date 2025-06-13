#ifndef COMMAND_H
#define COMMAND_H

typedef struct command Command;

Command *create_command(const char *first, const char *second, const char *third);

void free_command(Command *command);

const char *get_first_token(const Command *command);
const char *get_second_token(const Command *command);
const char *get_third_token(const Command *command);

#endif