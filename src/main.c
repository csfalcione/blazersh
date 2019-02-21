#include <stdio.h>
#include <stdlib.h>
#include <config.h>

#include "blazersh.h"


int main(int argc, char** argv) {

    puts ("Hello, world!");
    puts ("This is " PACKAGE_STRING ".");
    return 0;
}