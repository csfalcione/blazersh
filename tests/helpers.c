#include <check.h>

#include "../src/strarray.h"
#include "../src/parser.h"

void assert_strarray_eq(strarray* arr1, strarray* arr2) {
    int len1, len2;
    len1 = strarray_len(arr1);
    len2 = strarray_len(arr2);

    ck_assert_int_eq(len1, len2);

    for (int i = 0; i < len1; i++) {
        ck_assert_str_eq( strarray_get(arr1, i), strarray_get(arr2, i) ); 
    }
}

void assert_str_eq(char* a, char* b) {
    if (a == NULL || b == NULL) {
        ck_assert_ptr_eq(a, b);
        return;
    }
    ck_assert_str_eq(a, b);

}

void assert_tokenization(strarray* actual, char** expected, int length) {
    assert_strarray_eq(actual, strarray_from(expected, length));
}

void assert_strategy(execution_strategy actual, char** e_args, int e_length, char* e_input_file, char* e_output_file) {
    assert_tokenization(actual.args, e_args, e_length);
    assert_str_eq(actual.input_file, e_input_file);
    assert_str_eq(actual.output_file, e_output_file);
}