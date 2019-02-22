
typedef struct strarray strarray;

strarray* strarray_create_default();
strarray* strarray_create(int initialCapacity);


void strarray_add(strarray* arr, char* string);

char* strarray_get(strarray* arr, int index);

int strarray_len(strarray* arr);
int strarray_capacity(strarray* arr);

void strarray_free(strarray* arr);