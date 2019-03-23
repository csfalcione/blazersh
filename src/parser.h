#pragma once

#include "strarray.h"

typedef struct execution {
    strarray* args;
    char* input_file;
    int input_fd;
    char* output_file;
    int output_fd;
} execution_strategy;

execution_strategy parse(strarray* tokens);
