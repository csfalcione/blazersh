#include <stdlib.h>
#include <string.h>

#include "strarray.h"

int DEFAULT_INITIAL_CAPACITY = 4;

struct strarray {
    int length;
    int capacity;
    char** array;
};

strarray* strarray_create_default();
strarray* strarray_create(int initialCapacity);

void strarray_add(strarray* arr, char* string);

char* strarray_get(strarray* arr, int index);

int strarray_len(strarray* arr);
int strarray_capacity(strarray* arr);

void strarray_free(strarray* arr);

void strarray_upsize(strarray* arr, int newCapacity);
void free_strings(strarray* arr);


strarray* strarray_create_default() {
    return strarray_create(DEFAULT_INITIAL_CAPACITY);
}

strarray* strarray_create(int initialCapacity) {
    strarray* new = malloc( sizeof(strarray) );
    new->length = 0;
    new->array = malloc( sizeof(char*) * initialCapacity );
    new->capacity = initialCapacity;
    return new;
}


void strarray_add(strarray* arr, char* string) {
    if (arr->length == arr->capacity) {
        strarray_upsize(arr, arr->capacity * 1.5 + 1);
    }
    int strLen = strlen(string);
    char* copy = malloc( sizeof(char*) * (strLen + 1) );
    strcpy(copy, string);
    arr->array[arr->length] = copy;
    arr->length++;
}

void strarray_upsize(strarray* arr, int newCapacity) {
    int length = arr->length;
    char** oldArray = arr->array;
    char** newArray = malloc( sizeof(char*) * newCapacity );

    for (int i = 0; i < length; i++) {
        newArray[i] = oldArray[i];
    }
    arr->capacity = newCapacity;
    arr->array = newArray;

    free(oldArray);
}

char* strarray_get(strarray* arr, int index) {
    return arr->array[index];
}

int strarray_len(strarray* arr) {
    return arr->length;
}

int strarray_capacity(strarray* arr) {
    return arr->capacity;
}

void strarray_free(strarray* arr) {
    free_strings(arr);
    free(arr->array);
    free(arr);
}

void free_strings(strarray* arr) {
    char** array = arr->array;
    int length = arr->length;

    for (int i = 0; i < length; i++) {
        free( array[i] );
    }
}