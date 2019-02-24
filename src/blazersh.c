#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "blazersh.h"
#include "strarray.h"

enum parse_machine_state {Whitespace, Consuming, Escape, Quote};
typedef struct parse_state {
    enum parse_machine_state state;
    int cursor;
    char* working_copy;
    int copy_length;
    int copy_capacity;
} parse_state;

strarray* parse_input(char* input);
parse_state transition(parse_state state, char* input, strarray* result);
parse_state transition_whitespace(parse_state state, char* input, strarray* result);
parse_state transition_consuming(parse_state state, char* input, strarray* result);
parse_state transition_escape(parse_state state, char* input, strarray* result);
parse_state transition_quote(parse_state state, char* input, strarray* result);
parse_state add_char(parse_state state, char c);
int is_whitespace(char c);
int is_word(char c);


strarray* parse_input(char* input) {
    strarray* result = strarray_create_default();
    parse_state state = {
        .state = Whitespace,
        .cursor = 0,
        .working_copy = malloc( sizeof(char) * 41 ),
        .copy_length = 0,
        .copy_capacity = 41
    };
    int length = strlen(input);

    while (state.cursor < length) {
        state = transition(state, input, result);
    }
    if (state.copy_length > 0) {
        state = add_char(state, '\0');
        strarray_add(result, state.working_copy);
        state.copy_length = 0;
        state.working_copy[0] = '\0';
    }

    free(state.working_copy);
    return result;
}

parse_state transition(parse_state state, char* input, strarray* result) {
    switch (state.state) {

        case Whitespace:
            return transition_whitespace(state, input, result);
    
        case Consuming:
            return transition_consuming(state, input, result);
        
        case Escape:
            return transition_escape(state, input, result);

        case Quote:
            return transition_quote(state, input, result);
    }
}

parse_state transition_whitespace(parse_state state, char* input, strarray* result) {
    char c = input[state.cursor];
    parse_state next = state;
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

parse_state transition_consuming(parse_state state, char* input, strarray* result) {
    char c = input[state.cursor];
    parse_state next = state;
    next.cursor += 1;

    if (is_word(c)) {
        next = add_char(next, c);
    }
    if (is_whitespace(c)) {
        next = add_char(next, '\0');
        strarray_add(result, next.working_copy);
        next.copy_length = 0;
        next.working_copy[0] = '\0';
        next.state = Whitespace;
    }
    if (c == '\\') {
        next.state = Escape;
    }

    return next;

}

parse_state transition_escape(parse_state state, char* input, strarray* result) {
    char c = input[state.cursor];
    parse_state next = state;

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

parse_state transition_quote(parse_state state, char* input, strarray* result) {
    char c = input[state.cursor];
    parse_state next = state;

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

parse_state add_char(parse_state state, char c) {
    state.working_copy[state.copy_length] = c;
    state.copy_length++;

    if (state.copy_length < state.copy_capacity) return state;

    int newCapacity = state.copy_capacity * 1.5 + 1;
    state.working_copy = realloc(state.working_copy, sizeof(char) * newCapacity);
    state.copy_capacity = newCapacity;
    return state;
}
