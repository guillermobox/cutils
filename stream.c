#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char * readuntil(int fd, char token)
{
	static char buffer[128];
	char *pos;

	pos = buffer;

	while (read(fd, pos, 1)) {
		if (*pos == token) {
			*(++pos) = '\0';
			return buffer;
		}
		pos++;
	}

	if (pos == buffer)
		return NULL;

	return buffer;
}

char * readline(int fd) {
	return readuntil(fd, '\n');
}

int main(int argc, char *argv[])
{
	int fd;
	char * line;

	fd = open("Makefile", O_RDONLY);

	while (line = readline(fd)) {
		printf("[%3d] %s", strlen(line), line);
	};

	close(fd);

	return (EXIT_SUCCESS);
}

