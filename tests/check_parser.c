#include <check.h>

#include "../src/strarray.h"
#include "../src/parser.h"
#include "helpers.h"

START_TEST(test_parse)
{
    assert_strategy(
        parse_single(tokenize_input("echo hello "), 0, 2),
        (char* []) {"echo", "hello"}, 2, NULL, NULL
    );
    assert_strategy(
        parse_single(tokenize_input("echo hello > result.txt"), 0, 4),
        (char* []) {"echo", "hello"}, 2, NULL, "result.txt"
    );
    assert_strategy(
        parse_single(tokenize_input("grep -i < input.txt > result.txt"), 0, 6),
        (char* []) {"grep", "-i"}, 2, "input.txt", "result.txt"
    );
    assert_strategy(
        parse_single(tokenize_input("grep -i > result.txt < input.txt"), 0, 6),
        (char* []) {"grep", "-i"}, 2, "input.txt", "result.txt"
    );
    assert_strategy(
        parse_single(tokenize_input("grep -i > result.txt < input.txt"), 0, 6),
        (char* []) {"grep", "-i"}, 2, "input.txt", "result.txt"
    );
    assert_strategy(
        parse_single(tokenize_input("grep > result.txt -i < input.txt"), 0, 6),
        (char* []) {"grep", "-i"}, 2, "input.txt", "result.txt"
    );
}
END_TEST
