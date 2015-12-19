#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

darray primes;

void init_primes()
{
	primes = malloc(sizeof(darray*));
	darray_new(primes);
	darray_add(primes, 2);
	darray_add(primes, 3);
};

int is_prime(int x)
{
	int i, top, prime, lastprime;
	top = (int) sqrt(x);

	lastprime = primes->values[primes->used-1];
	if (x <= lastprime) {
		for (i = 0; i < primes->used; i++)
			if (primes->values[i] == x) return 1;
		return 0;
	};

	for (i = 0; i < primes->used; i++) {
		prime = primes->values[i];
		if (prime > top) return 1;
		if (x % prime == 0) return 0;
	};

	lastprime = prime;
	while (lastprime < top) {
		lastprime += 2;
		if (is_prime(lastprime)) {
			darray_add(primes, lastprime);
		}
	};

	for (; i < primes->used; i++) {
		prime = primes->values[i];
		if (prime > top) return 1;
		if (x % prime == 0) return 0;
	};

	return 1;
};

