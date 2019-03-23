
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tokenizer.h"
#include "parser.h"
#include "strarray.h"
#include "blazersh.h"

void handle_command(strarray* tokens);
void handle_environment();
void handle_get_variable(strarray* tokens);
void handle_set_variable(strarray* tokens);
void handle_list();
void handle_pwd();
void handle_cd();

int setup_strategy(execution_strategy* strategy);
int execute_strategy(execution_strategy strategy, int pipe_idx);
int cleanup_strategy(execution_strategy* strategy);
void close_pipes_except(execution_strategy strategy, int pipe_idx);

char* current_directory();
void route_command(strarray* tokens);
void print_strarray(strarray* arr);
const char* get_error_message(int err);


int handle_input(char* input) {
    strarray* tokens = tokenize_input(input);

    if (strarray_len(tokens) == 0) {
        return 0;
    }
    //these commands can't run in a subprocess and therefore don't support file redirection
    else if (strcmp( strarray_get(tokens, 0), "exit" ) == 0) {
        exit(0);
    }
    else if (strcmp( strarray_get(tokens, 0), "set" ) == 0) {
        handle_set_variable(tokens);
    }
    else if (strcmp( strarray_get(tokens, 0), "cd" ) == 0) {
        handle_cd(tokens);
    }
    else {
        handle_command(tokens);
    }
    strarray_free(tokens);

    return 0;
}

void handle_command(strarray* tokens) {
    execution_strategy strategy = parse(tokens);
    int open_res = setup_strategy(&strategy);

    if (open_res == -1) {
        puts( get_error_message(errno) );
        return;
    }

    pid_t pids[strategy.commands_length];

    for (int cmd_idx = 0; cmd_idx < strategy.commands_length; cmd_idx++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("fork error in command %d\n", cmd_idx);
        }

        if (pid == 0) { // child process
            execute_strategy(strategy, cmd_idx);
            route_command(strategy.commands[cmd_idx]);
            exit(errno);
        }
        //parent process
        pids[cmd_idx] = pid;
    }

    // close pipes
    close_pipes_except(strategy, -1); // close all pipes

    // wait for subprocesses
    for (int i = 0; i < strategy.commands_length; i++) {
        pid_t pid = pids[i];
        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status)) {
            printf("process for command %d didn't exit normally\n", i);
        }
    }
    cleanup_strategy(&strategy);
}

int setup_strategy(execution_strategy* strategy) {
    if (strategy->input_file != NULL) {
        strategy->input_fd = open(strategy->input_file, O_RDONLY);
    }
    if (strategy->output_file != NULL) {
        strategy->output_fd = open(strategy->output_file, O_CREAT | O_WRONLY, 0664);
    }
    if (strategy->input_fd == -1 || strategy->output_fd == -1) {
        cleanup_strategy(strategy);
        return -1;
    }

    for (int i = 0; i < strategy->commands_length - 1; i++) {
        int res = pipe(strategy->pipe_fds[i]);
        if (res != 0) {
            // close previously opened pipes
            printf("pipe error: %d\n", res);
            for (int j = i - 1; j >= 0; j--) {
                int* pipefd = strategy->pipe_fds[j];
                close(pipefd[0]);
                close(pipefd[1]);
            }
            cleanup_strategy(strategy);
            return -1;
        }
    }

    return 0;
}

int execute_strategy(execution_strategy strategy, int cmd_idx) {
    if (cmd_idx == 0 && strategy.input_file != NULL && strategy.input_fd >= 0) {
        //stdin from file
        dup2(strategy.input_fd, 0);
    }

    if (cmd_idx > 0) {
        //stdin from pipe
        printf("mapping stdin of command %d to pipe %d\n", cmd_idx, cmd_idx - 1);
        int* pipefd = strategy.pipe_fds[cmd_idx - 1];
        close(pipefd[1]);
        close_pipes_except(strategy, cmd_idx - 1);
        if (dup2(pipefd[0], 0) == -1) {
            printf("dup2 error: %s", get_error_message(errno));
        }
    }

    if (cmd_idx < strategy.commands_length - 1) {
        //stdout to pipe
        printf("mapping stdout of command %d to pipe %d\n", cmd_idx, cmd_idx);
        int* pipefd = strategy.pipe_fds[cmd_idx];
        close(pipefd[0]);
        close_pipes_except(strategy, cmd_idx);
        if (dup2(pipefd[1], 1) == -1) {
            printf("dup2 error: %s", get_error_message(errno));
        }
    }

    if (cmd_idx == strategy.commands_length - 1 && strategy.output_file != NULL && strategy.output_fd >= 0) {
        //stdout to file
        dup2(strategy.output_fd, 1);
    }
}

// pass anything negative for exclude_idx to close all pipes
void close_pipes_except(execution_strategy strategy, int exclude_idx) {
    for (int i = 0; i < strategy.commands_length - 1; i++) {
        if (i == exclude_idx) {
            continue;
        }
        // printf("closing pipe %d\n", i);
        int* pipefd = strategy.pipe_fds[i];
        close(pipefd[0]);
        close(pipefd[1]);
    }
}

int cleanup_strategy(execution_strategy* strategy) {
    if (strategy->input_file != NULL && strategy->input_fd >= 0) {
        close(strategy->input_fd);
    }
    if (strategy->output_file != NULL && strategy->output_fd >= 0) {
        close(strategy->output_fd);
    }

    // for (int i = 0; i < strategy->commands_length; i++) {
    //     strarray_free(strategy->commands[i]);
    // }
    // free(strategy->commands);
    // free(strategy->pipe_fds);

    return 0;
}

void route_command(strarray* tokens) {
    char* first_arg = strarray_get(tokens, 0);
    if (strcmp( first_arg , "help" ) == 0) {
        help();
    }
    else if (strcmp( first_arg, "environ" ) == 0) {
        handle_environment();
    }
    else if (strcmp( first_arg, "get" ) == 0) {
        handle_get_variable(tokens);
    }
    else if (strcmp( first_arg, "pwd" ) == 0) {
        handle_pwd();
    }
    else if (strcmp( first_arg, "list" ) == 0) {
        handle_list();
    }
    else if (strcmp( first_arg, "ls" ) == 0) {
        handle_list();
    }
    else {
        execute(tokens);
    }
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
        puts(get_error_message(errno));
        return;
    }
    puts(msg);
    free(msg);
}



void execute(strarray* tokens) {
    char** raw_array = strarray_unwrap(tokens);
    int result = execvp(raw_array[0], &raw_array[0]);
    if (result == -1) {
        puts(get_error_message(errno));
    }
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
If you have any problems, please report them to noreply@uab.edu"
    );
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

const char* get_error_message(int error) {
    switch (errno) {
        case EACCES:
            return "access denied";
        case EIO:
            return "I/O error";
        case EISDIR:
            return "filename is a directory";
        case EINVAL:
            return "invalid argument";
        case ELIBBAD:
            return "unrecognized format";
        case ELOOP:
            return "too many symbolic links encountered";
        case EMFILE:
            return "maximum number of open files for process exceeded";
        case ENFILE:
            return "maximum number of open files for system exceeded";
        case ENAMETOOLONG:
            return "filename too long";
        case ENOENT:
            return "file not found";
        case ENOEXEC:
            return "unrecognized format";
        case ENOTDIR:
            return "directory not found";
        case ENOMEM:
            return "insufficient kernel memory";
        case EFAULT:
            return "invalid data format";
        case EBADF:
            return "invalid file descriptor";
        default:
            return "unspecified error";
    }
}
