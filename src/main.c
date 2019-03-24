#include <stdlib.h>
#include <stdio.h>

#include "blazersh.h"

void print_prompt() {
    // the escape sequences make the prompt bold green
    printf("\n\033[1m\033[32m" "blazersh>" "\033[0m" " "); 
}

int main(int argc, char** argv) {
    for(;;) {
        print_prompt();
        char* input = get_input();

        int res = handle_input(input);
        if (res != 0) {
            return res;
        }

        free(input);
    }
}