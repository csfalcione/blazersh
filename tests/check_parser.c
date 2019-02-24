#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
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

void assert_parse_output(strarray* output, char** expected, int length) {
    assert_strarray_eq(output, strarray_from(expected, length));
}

START_TEST(test_parse)
{
    assert_parse_output(
        parse_input("exit "),
        (char* []) {"exit"}, 1
    );
    assert_parse_output(
        parse_input("\tls  -al "),
        (char* []) {"ls", "-al"}, 2
    );
    assert_parse_output(
        parse_input("\t"),
        (char* []) {}, 0
    );
    assert_parse_output(
        parse_input("cd \"My Documents\""),
        (char* []) {"cd", "My Documents"}, 2
    );
    assert_parse_output(
        parse_input("cd My\\ Documents"),
        (char* []) {"cd", "My Documents"}, 2
    );
}
END_TEST