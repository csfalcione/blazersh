#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blazersh.h"

void print_prompt(char* pwd, char* user) {
    char prompt_char = '$';
    if (strcmp(user, "root") == 0) {
        prompt_char = '#';
    }

    // the escape sequences influence color and style
    printf("\n\033[1m\033[32m" "blazersh\033[0m "  "%s %s\n" 
           "\033[1m" "%c " "\033[0m", user, pwd, prompt_char );
}

int main(int argc, char** argv) {
    for(;;) {
        char* pwd = current_directory();
        char* user = get_variable("USER");
        print_prompt(pwd, user);
        free(pwd);

        char* input = get_input();

        int res = handle_input(input);
        if (res != 0) {
            return res;
        }

        free(input);
    }
}