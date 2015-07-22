#include "hashset_open.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int inthash(unsigned int x)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x);
	return x & ((1 << HASHSIZE) -1);
}

hashset hashset_new()
{
	hashset hash = (hashset) calloc(1, sizeof(struct st_hashset));
	return hash;
};

void hashset_free(hashset hash)
{
	free(hash);
};

void hashset_add(hashset hash, int value)
{
	unsigned int index = inthash(value);
	unsigned int original = index;
	struct st_entry entry = {.value=value, .used=1};

	while (hash->entries[index].used == 1 && hash->entries[index].value != value) {
		index = (index + 1) % (1 << HASHSIZE);
		if (index == original) {
			fprintf(stderr, "Hash full!\n");
			exit(1);
		}
	};

	hash->entries[index] = entry;
};

int hashset_contains(hashset hash, int value)
{
	unsigned int index = inthash(value);
	while (hash->entries[index].used == 1) {
		if (hash->entries[index].value == value)
			return 1;
		index = (index + 1) % (1 << HASHSIZE);
	};
	return 0;
};

void hashset_dump(hashset hash)
{
	unsigned int i;
	for (i = 0; i < 1<<HASHSIZE; i++) {
		if (hash->entries[i].used != 0) {
			printf("%d ", hash->entries[i].value);
		}
	}
	printf("\n");
};

