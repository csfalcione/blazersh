
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "blazersh.h"
#include "parser.h"
#include "strarray.h"

int main_loop();
void execute(strarray* tokens);
strarray* environment();
char* get_variable(char* key);
char* set_variable(char* key, char* value);
strarray* list();
char* change_dir(char* directory);
char* help();

void handle_environment();
void handle_get_variable(strarray* tokens);
void handle_set_variable(strarray* tokens);
void handle_list();
void handle_pwd();
void handle_cd();

void print_prompt();
char* get_input();
void print_strarray(strarray* arr);
char* current_directory();


int main_loop() {

    for(;;) {
        print_prompt();
        char* input = get_input();
        strarray* tokens = parse_input(input);
        free(input);

        if (strarray_len(tokens) == 0) {
            continue;
        }
        else if (strcmp( strarray_get(tokens, 0), "exit" ) == 0) {
            return 0;
        }
        else if (strcmp( strarray_get(tokens, 0), "help" ) == 0) {
            help();
        }
        else if (strcmp( strarray_get(tokens, 0), "environ" ) == 0) {
            handle_environment();
        }
        else if (strcmp( strarray_get(tokens, 0), "get" ) == 0) {
            handle_get_variable(tokens);
        }
        else if (strcmp( strarray_get(tokens, 0), "set" ) == 0) {
            handle_set_variable(tokens);
        }
        else if (strcmp( strarray_get(tokens, 0), "pwd" ) == 0) {
            handle_pwd();
        }
        else if (strcmp( strarray_get(tokens, 0), "list" ) == 0) {
            handle_list();
        }
        else if (strcmp( strarray_get(tokens, 0), "ls" ) == 0) {
            handle_list();
        }
        else if (strcmp( strarray_get(tokens, 0), "cd" ) == 0) {
            handle_cd(tokens);
        } 
        else {
            execute(tokens);
        }
        strarray_free(tokens);
    }

}

void print_prompt() {
    printf("\nblazersh>"); 
}

void handle_environment() {
    strarray* env = environment();
    print_strarray( env );
    strarray_free(env);
}

void handle_get_variable(strarray* args) {
    if (strarray_len(args) < 2) {
        puts("Invalid arguments. Usage: get <var>");
        return;
    }
    puts(get_variable(strarray_get(args, 1)));
}

void handle_set_variable(strarray* args) {
    if (strarray_len(args) < 3) {
        puts("Invalid arguments. Usage: set <var> <val>");
        return;
    }
    puts(set_variable( strarray_get(args, 1), strarray_get(args, 2) ));
}

void handle_list() {
    print_strarray( list() );
}

void handle_pwd() {
    char* pwd = current_directory();
    puts(pwd);
    free(pwd);
}

void handle_cd(strarray* tokens) {
    if (strarray_len(tokens) < 2) {
        puts("Invalid arguments. Usage: cd <dir>");
        return;
    }
    char* msg = change_dir( strarray_get(tokens, 1) );
    if (msg == NULL) {
        switch (errno) {
            case EACCES:
                puts("permission denied");
                break;
            case EINVAL:
                puts("invalid argument");
                break;
            case EIO:
                puts("I/O error");
                break;
            case ENAMETOOLONG:
                puts("filename too long");
                break;
            case ENOENT:
                puts("no such file or directory");
                break;
            case ENOTDIR:
                puts("not a directory");
                break;
            case ELOOP:
                puts("too many symbolic links encountered when resolving path");
                break;
            default:
                puts("unspecified error");
                break;
        }
        return;
    }
    puts(msg);
    free(msg);
}



void execute(strarray* tokens) {
    // TODO
}


strarray* environment() {
    char** strings = __environ;
    strarray* result = strarray_create_default();
    for (int cursor = 0; strings[cursor] != NULL; cursor++) {
        strarray_add(result, strings[cursor]);
    }
    return result;
}


char* get_variable(char* var){
    char* result = getenv(var);
    if (result == NULL) {
        return "";
    }

    return result;
}


char* set_variable(char* var, char* value){
    setenv(var, value, 1);
    return get_variable(var);
}


strarray* list(){
    strarray* result = strarray_create_default();

    DIR* dir = opendir(".");

    if (dir == NULL) {
        strarray_add(result, "Current directory inaccesible.");
        return result;
    }

    struct dirent* dir_entry = readdir(dir);
    while (dir_entry != NULL) {
        strarray_add(result, dir_entry->d_name);
        dir_entry = readdir(dir);
    }

    closedir(dir);
    return result;
}


char* change_dir(char* directory) {
    int result = chdir(directory);
    if (result == -1) {
        return NULL;
    }

    char* pwd = current_directory();
    set_variable("PWD", pwd);

    return pwd;
}

void print_strarray(strarray* arr) {
    for(int i = 0; i < strarray_len(arr); i++) {
        puts( strarray_get(arr, i) );
    }
}

char* current_directory() {
    char* temp = malloc(sizeof(char) * 1024);
    char* res = getcwd(temp, 1024);
    return temp;
}


char* help(){
    puts("\
== Commands ==\n\
environ: list environment variables\n\
get <var>: show value for 'var' environment variable\n\
set <var> <val>: set 'var' environment variable to 'val'\n\
list: list files and subdirectories of the current directory\n\
ls: alias of list\n\
pwd: prints the present working directory\n\
cd <dir>: change the present working directory to 'dir'\n\
help: Show this text\n\
If you have any problems, please report them to noreply@uab.edu");
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
