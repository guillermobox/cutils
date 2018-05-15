#include <stdio.h>
#include "pmap.h"

int main(int argc, char *argv[])
{
	pmap directory;

	pmap_new(&directory, 8);
	pmap_set(&directory, "guillermo", strdup("R/ Entrepexigos 18, 1A"));

	printf("%s lives in \"%s\"\n", "guillermo", pmap_get(directory, "guillermo"));

	pmap_deepfree(&directory, free);
};
