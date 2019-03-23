
#include "../src/strarray.h"
#include "../src/parser.h"

void assert_strarray_eq(strarray* arr1, strarray* arr2);

void assert_str_eq(char* a, char* b);

void assert_tokenization(strarray* actual, char** expected, int length);

void assert_strategy(execution_strategy actual, char** e_args, int e_length, char* e_input_file, char* e_output_file);