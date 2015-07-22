/* has will have 2^HASHSIZE buckets */
#define HASHSIZE 18

struct st_entry {
	int value;
	char used;
	struct st_entry *next;
};

struct st_hashset {
	struct st_entry entries[1<<HASHSIZE];
};

typedef struct st_hashset * hashset;

hashset hashset_new();
void hashset_add(hashset hash, int value);
void hashset_dump(hashset hash);
void hashset_free(hashset hash);
int hashset_contains(hashset hash, int value);

