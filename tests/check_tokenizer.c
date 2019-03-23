#include <check.h>

#include "../src/strarray.h"
#include "../src/tokenizer.h"
#include "helpers.h"

START_TEST(test_tokenize)
{
    assert_tokenization(
        tokenize_input("exit "),
        (char* []) {"exit"}, 1
    );
    assert_tokenization(
        tokenize_input("\tls  -al "),
        (char* []) {"ls", "-al"}, 2
    );
    assert_tokenization(
        tokenize_input("\t"),
        (char* []) {}, 0
    );
    assert_tokenization(
        tokenize_input("cd \"My Documents\""),
        (char* []) {"cd", "My Documents"}, 2
    );
    assert_tokenization(
        tokenize_input("cp -u \"My Documents\" ~/doc_backup"),
        (char* []) {"cp", "-u", "My Documents", "~/doc_backup"}, 4
    );
    assert_tokenization(
        tokenize_input("cat  \"My Documents\" > result.txt"),
        (char* []) {"cat", "My Documents", ">", "result.txt"}, 4
    );
    assert_tokenization(
        tokenize_input("\"cd My Documents\""),
        (char* []) {"cd My Documents"}, 1
    );
    assert_tokenization(
        tokenize_input("cd \"My Documents"),
        (char* []) {"cd", "My Documents"}, 2
    );
    assert_tokenization(
        tokenize_input("cd My\\ Documents"),
        (char* []) {"cd", "My Documents"}, 2
    );
}
END_TEST