#include <stdlib.h>
#include <string.h>

#include "strarray.h"

int DEFAULT_INITIAL_CAPACITY = 4;

struct strarray {
    int length;
    int capacity;
    char** array;
};

void strarray_upsize(struct strarray* arr, int newCapacity);
void free_strings(struct strarray* arr);


struct strarray* strarray_create(int initialCapacity) {
    struct strarray* new = malloc( sizeof(struct strarray) );
    new->length = 0;
    new->array = malloc( sizeof(char*) * initialCapacity );
    new->capacity = initialCapacity;
    return new;
}

struct strarray* strarray_create_default() {
    return strarray_create(DEFAULT_INITIAL_CAPACITY);
}

void strarray_add(struct strarray* arr, char* string) {
    if (arr->length == arr->capacity) {
        strarray_upsize(arr, arr->capacity * 1.5 + 1);
    }
    int strLen = strlen(string);
    char* copy = malloc( sizeof(char*) * (strLen + 1) );
    strcpy(copy, string);
    arr->array[arr->length] = copy;
    arr->length++;
}

void strarray_upsize(struct strarray* arr, int newCapacity) {
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

char* strarray_get(struct strarray* arr, int index) {
    return arr->array[index];
}

int strarray_len(struct strarray* arr) {
    return arr->length;
}

int strarray_capacity(struct strarray* arr) {
    return arr->capacity;
}

void strarray_free(struct strarray* arr) {
    free_strings(arr);
    free(arr->array);
    free(arr);
}

void free_strings(struct strarray* arr) {
    char** array = arr->array;
    int length = arr->length;

    for (int i = 0; i < length; i++) {
        free( array[i] );
    }
}