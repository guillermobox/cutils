#include "darray.h"

/*
 * Dynamic array
 *
 * This array grows as elements are added
 */
void darray_alloc(struct st_darray * a)
{
	a->alloc = DARRAY_INITIAL_SIZE;
	a->used = 0;
	a->values = malloc(a->alloc * sizeof(DARRAY_TYPE));
}

void darray_free(struct st_darray * a)
{
	free(a->values);
}

void darray_insert(struct st_darray * a, DARRAY_TYPE val)
{
	if (a->used == a->alloc) {
		a->alloc *= 2;
		a->values = realloc(a->values, a->alloc * sizeof(DARRAY_TYPE));
	}
	a->values[a->used++] = val;
}

