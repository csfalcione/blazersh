
struct string_array;

struct string_array strarray_create();
struct string_array strarray_create(int initialCapacity);


void strarray_add(struct string_array* arr, char* string);

char* strarray_get(struct string_array* arr, int index);