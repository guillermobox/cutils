#include "hashset_linked.h"

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

static void entry_free(struct st_entry * entry)
{
	if (entry == NULL)
		return;
	entry_free(entry->next);
	free(entry);
};

void hashset_free(hashset hash)
{
	unsigned int i;
	for (i = 0; i < (1<<HASHSIZE); i++) {
		struct st_entry * entry = hash->entries[i];
		entry_free(entry);
	}
	free(hash);
};

static struct st_entry * new_entry(int value)
{
	struct st_entry * entry = malloc(sizeof(struct st_entry));
	entry->value = value;
	entry->next = NULL;
	return entry;
};

void hashset_add(hashset hash, int value)
{
	unsigned int index = inthash(value);

	if (hash->entries[index] == NULL) {
		hash->entries[index] = new_entry(value);
	} else {
		struct st_entry * entry;
		entry = hash->entries[index];
		if (entry->value == value) return;
		while (entry->next) {
			if (entry->value == value) return;
			entry = entry->next;
		};
		entry->next = new_entry(value);
	}
};

int hashset_contains(hashset hash, int value)
{
	unsigned int index = inthash(value);
	struct st_entry * entry = hash->entries[index];
	while (entry != NULL) {
		if (entry->value == value)
			return 1;
		entry = entry->next;
	}
	return 0;
};

void hashset_dump(hashset hash)
{
	unsigned int i;
	for (i = 0; i < 1<<HASHSIZE; i++) {
		struct st_entry * entry = hash->entries[i];
		while (entry != NULL) {
			printf("%d ", entry->value);
			entry = entry->next;
		}
	}
	printf("\n");
};

