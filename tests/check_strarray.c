
#include <check.h>

#include "../src/strarray.h"


START_TEST(test_strarray)
{
    strarray* arr = strarray_create(2);


    ck_assert_int_eq( strarray_len(arr), 0 );

    strarray_add(arr, "hello");
    ck_assert_int_eq( strarray_len(arr), 1 );
    ck_assert_str_eq( strarray_get(arr, 0), "hello" );

    strarray_add(arr, "world");
    strarray_add(arr, "should resize here");

    strarray_set(arr, 1, "WORLD");

    ck_assert_int_eq( strarray_len(arr), 3 ); //verify length
    ck_assert_int_gt( strarray_capacity(arr), 2 ); //verify resize
    ck_assert_str_eq( strarray_get(arr, 0), "hello" ); //verify resize didn't mangle old data
    ck_assert_str_eq( strarray_get(arr, 1), "WORLD" ); //verify set
    ck_assert_str_eq( strarray_get(arr, 2), "should resize here" ); //verify add?
    

    strarray_free(arr);
}
END_TEST

START_TEST(test_strarray_from)
{
    char* initial[3] = {"test1", "test2", "test3"};
    strarray* arr = strarray_from(initial, 3);

    ck_assert_str_eq( strarray_get(arr, 0), "test1" );
    ck_assert_str_eq( strarray_get(arr, 1), "test2" );
    ck_assert_str_eq( strarray_get(arr, 2), "test3" );
}
END_TEST