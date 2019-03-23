#include <stdlib.h>
#include <stdio.h>

#include "blazersh.h"

void print_prompt() {
    printf("\nblazersh>"); 
}

int main(int argc, char** argv) {
    for(;;) {
        char* input = get_input();

        int res = handle_input(input);
        if (res >= 0) {
            return res;
        }

        free(input);
    }
}