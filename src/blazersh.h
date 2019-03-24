
#include "strarray.h"

char* get_input();
int handle_input(char* input);

void execute(strarray* args);

strarray* environment();
char* get_variable(char* key);
char* set_variable(char* key, char* value);

strarray* list();

char* change_dir(char* directory);

char* help();
