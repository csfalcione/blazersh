
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "strarray.h"

enum tokenize_machine_state {Whitespace, Consuming, Escape, Quote};
typedef struct tokenize_state {
    enum tokenize_machine_state state;
    int cursor;
    char* working_copy;
    int copy_length;
    int copy_capacity;
} tokenize_state;

enum parse_machine_state {Argument, Input, Output};

execution_strategy parse(strarray* tokens);

strarray* tokenize_input(char* input);

tokenize_state emit_token(tokenize_state state, strarray* tokens);
tokenize_state add_char(tokenize_state state, char c);

tokenize_state transition(tokenize_state state, char* input, strarray* tokens);
tokenize_state transition_whitespace(tokenize_state state, char* input, strarray* tokens);
tokenize_state transition_consuming(tokenize_state state, char* input, strarray* tokens);
tokenize_state transition_escape(tokenize_state state, char* input, strarray* tokens);
tokenize_state transition_quote(tokenize_state state, char* input, strarray* tokens);

int is_whitespace(char c);
int is_word(char c);


execution_strategy parse(strarray* tokens) {
    execution_strategy strategy = {
        .args = strarray_create( strarray_len(tokens) ),
        .input_file = NULL,
        .input_fd = -1,
        .output_file = NULL,
        .output_fd = -1,
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


strarray* tokenize_input(char* input) {
    strarray* tokens = strarray_create_default();
    tokenize_state state = {
        .state = Whitespace,
        .cursor = 0,
        .working_copy = malloc( sizeof(char) * 41 ),
        .copy_length = 0,
        .copy_capacity = 41
    };
    int length = strlen(input);

    while (state.cursor < length) {
        state = transition(state, input, tokens);
    }
    if (state.copy_length > 0) {
        state = emit_token(state, tokens);
    }

    free(state.working_copy);
    return tokens;
}


tokenize_state emit_token(tokenize_state state, strarray* tokens) {
    state = add_char(state, '\0');
    strarray_add(tokens, state.working_copy);
    state.copy_length = 0;
    state.working_copy[0] = '\0';
    return state;
}

tokenize_state add_char(tokenize_state state, char c) {
    state.working_copy[state.copy_length] = c;
    state.copy_length++;

    if (state.copy_length < state.copy_capacity) return state;

    int newCapacity = state.copy_capacity * 1.5 + 1;
    state.working_copy = realloc(state.working_copy, sizeof(char) * newCapacity);
    state.copy_capacity = newCapacity;
    return state;
}


tokenize_state transition(tokenize_state state, char* input, strarray* tokens) {
    switch (state.state) {

        case Whitespace:
            return transition_whitespace(state, input, tokens);
    
        case Consuming:
            return transition_consuming(state, input, tokens);
        
        case Escape:
            return transition_escape(state, input, tokens);

        case Quote:
            return transition_quote(state, input, tokens);
    }
}

tokenize_state transition_whitespace(tokenize_state state, char* input, strarray* tokens) {
    char c = input[state.cursor];
    tokenize_state next = state;
    next.cursor += 1;

    if (is_word(c)) {
        next = add_char(next, c);
        next.state = Consuming;
    }
    if (c == '\\') {
        next.state = Escape;
    }
    if (c == '"') {
        next.state = Quote;
    }

    return next;
}

tokenize_state transition_consuming(tokenize_state state, char* input, strarray* tokens) {
    char c = input[state.cursor];
    tokenize_state next = state;
    next.cursor += 1;

    if (is_word(c)) {
        next = add_char(next, c);
    }
    if (is_whitespace(c)) {
        next = emit_token(next, tokens);
        next.state = Whitespace;
    }
    if (c == '\\') {
        next.state = Escape;
    }

    return next;

}

tokenize_state transition_escape(tokenize_state state, char* input, strarray* tokens) {
    char c = input[state.cursor];
    tokenize_state next = state;

    switch (c) {

        case 'n':
            next = add_char(next, '\n');
            break;
        case 'r':
            next = add_char(next, '\r');
            break;
        case 't':
            next = add_char(next, '\t');
            break;

        default:
            next = add_char(next, c);
    }

    next.cursor += 1;
    next.state = Consuming;
    return next;
}

tokenize_state transition_quote(tokenize_state state, char* input, strarray* tokens) {
    char c = input[state.cursor];
    tokenize_state next = state;

    next.cursor += 1;
    if (c == '"') {
        next.state = Consuming;
        return next;
    }

    if (c == '\\') {
        next.state = Escape;
        return next;
    }

    next = add_char(next, c);
    return next;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}

int is_word(char c) {
    return !is_whitespace(c) && c != '\\' && c != '\n' && c != '"';
}
