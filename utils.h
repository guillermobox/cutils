#include <stdlib.h>

struct st_dynamic_array {
	size_t alloc;
	size_t used;
	int * values;
};

typedef struct st_dynamic_array darray;

int is_prime(int x);
void darray_add(darray *a, int val);
void darray_free(darray *a);
void darray_alloc(darray *a);
