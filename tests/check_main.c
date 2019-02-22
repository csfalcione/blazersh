#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <check.h>

#include "check_strarray.c"
#include "check_parser.c"
#include "check_blazersh.c"

Suite* blazersh_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Parsing");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_strarray);
    tcase_add_test(tc_core, test_strarray_from);
    tcase_add_test(tc_core, test_parse); 
    suite_add_tcase(s, tc_core);

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
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}