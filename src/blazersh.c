#include "blazersh.h"

struct string_array {
    int length;
    int capacity;
    char** array;
};