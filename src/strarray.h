
typedef struct strarray strarray;

strarray* strarray_create_default();
strarray* strarray_create(int initialCapacity);
strarray* strarray_from(char** array, int length);

void strarray_add(strarray* arr, char* string);

char* strarray_get(strarray* arr, int index);
void strarray_set(strarray* arr, int index, char* value);

int strarray_len(strarray* arr);
int strarray_capacity(strarray* arr);

void strarray_free(strarray* arr);

char** strarray_unwrap(strarray* arr);

void print_strarray(strarray* arr);