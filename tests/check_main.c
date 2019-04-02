#include <check.h>

#include "check_strarray.c"
#include "check_tokenizer.c"
#include "check_parser.c"

Suite* blazersh_suite(void)
{

    Suite* s = suite_create("Parsing");

    TCase* tc_strarray = tcase_create("String Array");
    TCase* tc_strarray_from = tcase_create("String Array - from");
    TCase* tc_strarray_join = tcase_create("String Array - join");
    TCase* tc_tokenize = tcase_create("Tokenizing User Input");
    TCase* tc_parse = tcase_create("Parsing User Input");

    tcase_add_test(tc_strarray, test_strarray);
    tcase_add_test(tc_strarray_from, test_strarray_from);
    tcase_add_test(tc_strarray_join, test_strarray_join);
    tcase_add_test(tc_tokenize, test_tokenize); 
    tcase_add_test(tc_parse, test_parse); 

    suite_add_tcase(s, tc_strarray);
    suite_add_tcase(s, tc_strarray_from);
    suite_add_tcase(s, tc_strarray_join);
    suite_add_tcase(s, tc_tokenize);
    suite_add_tcase(s, tc_parse);

    return s;
}

int main() {

    int number_failed;
    Suite* s;
    SRunner* sr;

    s = blazersh_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}