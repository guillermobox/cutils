#include <string.h>
#include "pmap.h"

static unsigned int hash(const char *s, unsigned int hashsize)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % hashsize;
};

void pmap_new(struct st_pmap *map, size_t initiallength)
{
	map->entries = calloc(initiallength, sizeof(struct st_pmap_entry));
	if (map->entries == NULL) {
	}
	map->length = initiallength;
};

void pmap_set(struct st_pmap *map, const char *key, void *pointer)
{
	size_t index, start;

	start = index = hash(key, map->length);

	while (map->entries[index].key != NULL) {
		if (strcmp(map->entries[index].key, key) == 0) {
			map->entries[index].pointer = pointer;
			return;
		};
		index = (index + 1) % map->length;
		if (index == start) {
		}
	};

	map->entries[index].pointer = pointer;
	map->entries[index].key = strdup(key);
};

void * pmap_get(struct st_pmap map, const char *key)
{
	size_t index;

	index = hash(key, map.length);

	while (map.entries[index].key != NULL) {
		if (strcmp(map.entries[index].key, key) == 0) {
			return map.entries[index].pointer;
		};
		index = (index + 1) % map.length;
	};
	return NULL;
};

void pmap_free(struct st_pmap *map)
{
	size_t index;

	for (index = 0; index < map->length; index++) {
		if (map->entries[index].key != NULL) {
			free(map->entries[index].key);
		};
	};

	free(map->entries);
};

void pmap_deepfree(struct st_pmap *map, void(*freefunction)(void*))
{
	size_t index;

	for (index = 0; index < map->length; index++) {
		if (map->entries[index].key != NULL) {
			free(map->entries[index].key);
			freefunction(map->entries[index].pointer);
		};
	};

	free(map->entries);
};
