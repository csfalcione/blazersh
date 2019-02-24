
#include "strarray.h"

int main_loop();

void execute(strarray* tokens);

strarray* environment();
char* get_variable(char* key);
char* set_variable(char* key, char* value);

strarray* list();

void change_dir(char* directory);

char* help();
