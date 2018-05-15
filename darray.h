#include <stdlib.h>

/*
 * Dynamic array
 *
 * This array grows as elements are added
 *
 * To use it:
 * 	Create a structure st_darray
 * 	Initialize the structure with darray_aloc and destroy it with darray_free
 * 	Append elements to the end of the array with darray_insert
 * 	Access elements directly using the .values pointer, up to .used
 */

/* library configuration macros */
#ifndef DARRAY_TYPE
#define DARRAY_TYPE int
#endif
#ifndef DARRAY_INITIAL_SIZE
#define DARRAY_INITIAL_SIZE ( (size_t) 128UL )
#endif

struct st_darray {
	size_t alloc;
	size_t used;
	DARRAY_TYPE * values;
};

/* library interface */
void darray_alloc(struct st_darray * a);
void darray_free(struct st_darray * a);
void darray_insert(struct st_darray * a, DARRAY_TYPE val);

