#pragma once

#include "strarray.h"
#include <stdbool.h>

typedef struct execution {
    bool background;
    strarray** commands;
    int commands_length;
    int** pipe_fds;
    char* input_file;
    int input_fd;
    char* output_file;
    int output_fd;
} single_strategy;

typedef struct multi_strategy {
    single_strategy* subcommands;
    int subcommands_length;
} multi_strategy;

multi_strategy parse(strarray* tokens);
single_strategy parse_single(strarray* tokens, int start_idx, int end_idx);

void free_strategy(multi_strategy strategy);
