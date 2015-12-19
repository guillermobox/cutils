#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>

#define SHOW(function, arguments) printf("%s%s = %d\n", #function, #arguments, function arguments);

void init_primes();
int is_prime(int x);

/*
 * Set
 *
 * A set implemented using a binary tree.
 */
typedef btree set;

#define set_add btree_unique_add
#define set_new btree_new_node
#define set_show btree_show

#endif
