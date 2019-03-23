
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "strarray.h"

enum parse_machine_state {Argument, Input, Output};

execution_strategy parse(strarray* tokens);


execution_strategy parse(strarray* tokens) {
    execution_strategy strategy = {
        .args = strarray_create( strarray_len(tokens) ),
        .input_file = NULL,
        .input_fd = -153, //negative and not -1
        .output_file = NULL,
        .output_fd = -153,
    };

    enum parse_machine_state state = Argument;

    for (int i = 0; i < strarray_len(tokens); i++) {
        char* token = strarray_get(tokens, i);
        if (strcmp(token, ">") == 0) {
            state = Output;
            continue;
        }
        if (strcmp(token, "<") == 0) {
            state = Input;
            continue;
        }
        if (state == Argument) {
            strarray_add(strategy.args, token);
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
