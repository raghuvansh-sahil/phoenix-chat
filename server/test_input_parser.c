#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "input_parser.h"
#include "command.h"

void test_null_input() {
    Command *command = parse_input(NULL);
    assert(command == NULL);
}

void test_empty_input() {
    Command *command = parse_input("");
    assert(command == NULL);
}

void test_single_token_input() {
    Command *command = parse_input("/logout");
    
    assert(command != NULL);
    assert(strcmp(get_first_token(command), "/logout") == 0);
    assert(get_second_token(command) == NULL);
    assert(get_third_token(command) == NULL);

    free_command(command);
}

void test_double_token_input() {
    Command *command = parse_input("/login raghuvansh-sahil");

    assert(command != NULL);
    assert(strcmp(get_first_token(command), "/login") == 0);
    assert(strcmp(get_second_token(command), "raghuvansh-sahil") == 0);
    assert(get_third_token(command) == NULL);

    free_command(command);
}

void test_triple_token_input() {
    Command *command = parse_input("/send raghuvansh-sahil A quick brown fox jumps over a lazy dog.");
    
    assert(command != NULL);
    assert(strcmp(get_first_token(command), "/send") == 0);
    assert(strcmp(get_second_token(command), "raghuvansh-sahil") == 0);
    assert(strcmp(get_third_token(command), "A quick brown fox jumps over a lazy dog.") == 0);

    free_command(command);
}

int main() {
    printf("Running unit tests...\n");

    test_null_input();
    test_empty_input();
    test_single_token_input();
    test_double_token_input();
    test_triple_token_input();

    printf("All tests passed.\n");
    return 0;
}