
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "blazersh.h"
#include "parser.h"
#include "strarray.h"

int main_loop();
void print_prompt();
char* get_input();


int main_loop() {

    for(;;) {
        print_prompt();
        char* input = get_input();
        strarray* tokens = parse_input(input);

        if (strarray_len(tokens) == 0) {
            printf("\n");
            continue;
        }

        if (strcmp( strarray_get(tokens, 0), "exit" ) == 0) {
            return 0;
        }

    }

}

void print_prompt() {
    printf("\nblazersh>"); 
}

char* get_input() {
    size_t capacity = 80;
    char* result = malloc(sizeof(char) * capacity);
    int c;
    size_t length = 0;
    while (EOF != (c = getchar()) && c != '\r' && c != '\n') {
        result[length] = c;
        length++;
        if(length == capacity - 1){
            capacity *= 2;
            result = realloc(result, sizeof(char) * capacity);
        }
    }
    length++;
    result[length] = '\0';
    return realloc(result, sizeof(char) * length);
}
