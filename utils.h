#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>

#define SHOW(function, arguments) printf("%s%s = %d\n", #function, #arguments, function arguments);
void init_primes();
int is_prime(int x);

/*
 * Dynamic array
 *
 * This array grows as elements are added
 */
#ifndef DARRAY_TYPE
#define DARRAY_TYPE int
#endif

struct st_dynamic_array {
	size_t alloc;
	size_t used;
	DARRAY_TYPE * values;
};

typedef struct st_dynamic_array * darray;

void darray_new(darray a);
void darray_destroy(darray a);
void darray_add(darray a, DARRAY_TYPE val);

/*
 * Binary tree
 *
 * Binary tree implementation
 */
#ifndef TREE_TYPE
#define TREE_TYPE int
#endif

struct st_btree_node {
	struct st_btree_node *left;
	struct st_btree_node *right;
	TREE_TYPE value;
};

typedef struct st_btree_node *btree;

btree btree_new_node(TREE_TYPE val);
void  btree_destroy(btree root);
btree btree_add_left(btree root, TREE_TYPE val);
btree btree_add_right(btree root, TREE_TYPE val);
btree btree_sorted_add(btree root, TREE_TYPE val);
int   btree_check(btree root, TREE_TYPE val);
void  btree_dotshow(btree root);
void  btree_show(btree root);
void  btree_plain_show(btree root);
btree btree_unique_add(btree root, TREE_TYPE val);
int   btree_length(btree root);

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
