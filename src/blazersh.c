#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "blazersh.h"
#include "strarray.h"

strarray* parse(char* input);

strarray* parse(char* input) {
    return strarray_from((char* []) {"hello"}, 1);
}