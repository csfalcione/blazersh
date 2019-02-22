
struct strarray;

struct strarray* strarray_create_default();
struct strarray* strarray_create(int initialCapacity);


void strarray_add(struct strarray* arr, char* string);

char* strarray_get(struct strarray* arr, int index);

int strarray_len(struct strarray* arr);
int strarray_capacity(struct strarray* arr);

void strarray_free(struct strarray* arr);