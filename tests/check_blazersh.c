#include <config.h>
#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include "../src/blazersh.h"

START_TEST(test_blazersh_parse)
{
    ck_assert_int_eq(0, 0);
}
END_TEST

Suite* parse_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Parsing");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_blazersh_parse);
    suite_add_tcase(s, tc_core);

    return s;
}

int main() {
    int number_failed;
    Suite* s;
    SRunner* sr;

    s = parse_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}