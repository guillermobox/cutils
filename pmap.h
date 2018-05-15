#ifndef _PMAP_H_
#define _PMAP_H_

/*
 * PMAP is a map of strings to pointers
 *
 * Create a new map of certain size with pmap_new() Set a key/value pair using
 * pmap_set, get the value back using pmap_get.  Finally, pmap_free will free
 * the structure, and pmap_deepfree allows to also free all the pointers.
 */

#include <stdlib.h>

struct st_pmap {
	struct st_pmap_entry * entries;
	size_t length;
};

struct st_pmap_entry {
	char * key;
	void * pointer;
};

typedef struct st_pmap pmap;

void  pmap_new(pmap *map, size_t initiallength);
void  pmap_set(pmap *map, const char *key, void *pointer);
void *pmap_get(pmap map, const char *key);
void  pmap_free(pmap *map);
void  pmap_deepfree(pmap *map, void (*f)(void*));

#endif
