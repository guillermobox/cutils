#include <stdio.h>

struct st_iterator {
	unsigned int i;
	int (*filter)(unsigned int);
};

void iterator_start(struct st_iterator *iter, int (*filter)(unsigned int))
{
	iter->i = 0;
	iter->filter = filter;
}

unsigned int iterator_next(struct st_iterator * iter)
{
	if (iter->filter != NULL) {
		while (iter->filter(iter->i) == 0) {
			iter->i++;
		}
	}
	return iter->i++;
}

int filter_pairs(unsigned int x)
{
	return x%2 == 0;
}

#define FOR_ITERATOR(filter, variable) \
	struct st_iterator _iterator; \
	unsigned int variable; \
	iterator_start(&_iterator, filter); \
	for (;; variable = iterator_next(&_iterator))

int main(int argc, char *argv[])
{
	FOR_ITERATOR(filter_pairs, elemento) {
		printf("%d\n", elemento);
		if (elemento > 100) break;
	}

	return 0;
}
