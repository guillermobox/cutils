#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* common interface for the hashset library */
typedef void * hashset;

hashset hashset_new();
void hashset_add(hashset hash, int value);
void hashset_dump(hashset hash);
void hashset_free(hashset hash);
int hashset_contains(hashset hash, int value);

/* testing main program */
int main(int argc, char *argv[])
{
	hashset h;

	h = hashset_new();
	
	if (argc != 1) {
		char **arg;
		arg = argv + 1;
		while (*arg) {
			hashset_add(h, atoi(*arg));
			arg++;
		}
		hashset_dump(h);
	} else {
		int i, imax;
		i = imax = 32768;
		printf("No input arguments, inserting %d random numbers!\n", imax);
		srand(time(NULL));
		while (i--) {
			hashset_add(h, rand() % imax);
		}
		printf("Contains the 42? %s\n", hashset_contains(h, 42) ? "yes" : "no");
	}

	hashset_free(h);
	return 0;
};
