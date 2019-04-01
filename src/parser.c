
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "strarray.h"

enum parse_machine_state {Argument, Input, Output};

multi_strategy init_multi_strategy (strarray* tokens);
single_strategy init_single_strategy (strarray* tokens, int start_idx, int end_idx);
void free_single(single_strategy strategy);

multi_strategy parse(strarray* tokens) {
    multi_strategy strategy = init_multi_strategy(tokens);
    int subcommand_idx = 0;

    int start_idx = 0;
    for (int end_idx = 0; end_idx < strarray_len(tokens); end_idx++) {
        // loop invariant: strategy.subcommands[subcommand_idx] hasn't been assigned to

        if(strcmp(strarray_get(tokens, end_idx), "&") != 0) {
            continue;
        }

        single_strategy subcommand = parse_single(tokens, start_idx, end_idx);
        subcommand.background = true;
        strategy.subcommands[subcommand_idx] = subcommand;
        start_idx = end_idx + 1; // +1 to skip the ampersand
        subcommand_idx++;
    }

    if (start_idx < strarray_len(tokens)) {
        single_strategy subcommand = parse_single(tokens, start_idx, strarray_len(tokens));
        subcommand.background = false;
        strategy.subcommands[subcommand_idx] = subcommand;
    }

    return strategy;

}

single_strategy parse_single(strarray* tokens, int start_idx, int end_idx) {
    single_strategy strategy = init_single_strategy(tokens, start_idx, end_idx);

    enum parse_machine_state state = Argument;
    int command_idx = 0;

    for (int i = start_idx; i < end_idx; i++) {
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

multi_strategy init_multi_strategy (strarray* tokens) {
    int num_tokens = strarray_len(tokens);
    int num_commands = 0;
    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(strarray_get(tokens, i), "&") == 0) {
            num_commands++;
        }
    }
    if (strcmp(strarray_get(tokens, num_tokens - 1), "&") != 0) {
        // if the last token isn't an ampersand then there is one more command
        // i.e. <command> & <command> & <command>
        num_commands++;
    }

    single_strategy* subcommands = malloc( sizeof(single_strategy) * num_commands );
    multi_strategy res = {
        .subcommands = subcommands,
        .subcommands_length = num_commands
    };
    return res;
}

single_strategy init_single_strategy (strarray* tokens, int start_idx, int end_idx) {
    int pipe_count = 0;
    for (int i = start_idx; i < end_idx; i++) {
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

    single_strategy res = {
        .commands = commands,
        .commands_length = commands_length,
        .pipe_fds = pipe_fds,
        .input_file = NULL,
        .input_fd = -153, //negative and not -1
        .output_file = NULL,
        .output_fd = -153,
        .background = false
    };
    return res;
}

void free_strategy(multi_strategy strategy) {
    for (int i = 0; i < strategy.subcommands_length; i++) {
        free_single(strategy.subcommands[i]);
    }
}

void free_single(single_strategy strategy) {
    int num_commands = strategy.commands_length;

    //cleanup commands
    strarray** commands = strategy.commands;
    for (int i = 0; i < num_commands; i++) {
        strarray_free(commands[i]);
    }
    free(commands);

    //cleanup pipe_fds
    int** pipe_fds = strategy.pipe_fds;
    for (int i = 0; i < num_commands - 1; i++) {
        free(pipe_fds[i]);
    }
    free(pipe_fds);
}
