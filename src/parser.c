
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "strarray.h"

enum parse_machine_state {Argument, Input, Output};

execution_strategy init_strategy (strarray* tokens);


execution_strategy parse(strarray* tokens) {
    execution_strategy strategy = init_strategy(tokens);

    enum parse_machine_state state = Argument;
    int command_idx = 0;

    for (int i = 0; i < strarray_len(tokens); i++) {
        char* token = strarray_get(tokens, i);
        if (strcmp(token, "|") == 0) {
            state = Argument;
            command_idx++;
            continue;
        }
        if (strcmp(token, ">") == 0) {
            state = Output;
            continue;
        }
        if (strcmp(token, "<") == 0) {
            state = Input;
            continue;
        }
        if (state == Argument) {
            strarray_add(strategy.commands[command_idx], token);
        }
        if (state == Input) {
            strategy.input_file = token;
            state = Argument;
        }
        if (state == Output) {
            strategy.output_file = token;
            state = Argument;
        }
    }
    return strategy;
}

execution_strategy init_strategy (strarray* tokens) {
    int pipe_count = 0;
    for (int i = 0; i < strarray_len(tokens); i++) {
        char* token = strarray_get(tokens, i);
        if (strcmp(token, "|") == 0) {
            pipe_count++;
        }
    }
    int commands_length = pipe_count + 1;

    strarray** commands = malloc( sizeof(strarray*) * (commands_length + 1) );
    commands[commands_length] = NULL;
    for (int i = 0; i < commands_length; i++) {
        commands[i] = strarray_create_default();
    }

    int** pipe_fds = malloc( sizeof(int*) * (pipe_count) );

    for (int i = 0; i < pipe_count; i++) {
        pipe_fds[i] = malloc(sizeof(int) * 2);
    }

    execution_strategy res = {
        .commands = commands,
        .commands_length = commands_length,
        .pipe_fds = pipe_fds,
        .input_file = NULL,
        .input_fd = -153, //negative and not -1
        .output_file = NULL,
        .output_fd = -153,
    };
    return res;
}
