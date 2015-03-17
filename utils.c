#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

struct st_list {
	int * value;
	int size;
};

typedef struct st_list * List;

List newList(void){
	List l;
	l = malloc(sizeof(struct st_list));
	l->value = NULL;
	l->size = 0;
	return l;
}

void delList(List list){
	if(list!=NULL){
		if(list->value!=NULL)
			free(list->value);
		free(list);
	}
}

int popList(List list, int index){
	int value, i, j;
	if( index >= list->size ){
		printf("WOP! Accesing element %d of a %d list\n", index, list->size);
		exit(1);
	}
	value = list->value[index];
	i=0; j=0;
	if(index != list->size-1){
		for(i=0; i<list->size; i++,j++){
			if(j == index) j = j+1;
			list->value[i] = list->value[j];
		}
	}
	list->size -= 1;
	return value;
}

int pushList(List list, int value){
	list->size += 1;
	list->value = realloc(list->value, list->size * sizeof(int));
	list->value[list->size-1] = value;
}

void showList(List list){
	int i;
	for(i=0; i < list->size; i++)
		printf("%d ", list->value[i]);
	printf("\n");
}

#define MAXPRIMES 200000

int nprimes;
int primelist[MAXPRIMES];

int isprime(int n){
	int top, i, prime;
	top = (int)sqrt(n);
	for(i=0; i<nprimes; i++){
		prime = primelist[i];
		if( prime > top ) break;
		if( n%prime == 0 ) return 0;
	}
	if (prime < top)
	return 1;
}


void darray_alloc(darray *a)
{
	a->alloc = 64;
	a->used = 0;
	a->values = malloc(a->alloc * sizeof(int));
};

void darray_free(darray *a)
{
	free(a->values);
};

void darray_add(darray *a, int val)
{
	if (a->used == a->alloc) {
		a->alloc *= 2;
		a->values = realloc(a->values, a->alloc * sizeof(int));
	}
	a->values[a->used++] = val;
};

darray primes;

void init_primes()
{
	darray_alloc(&primes);
	darray_add(&primes, 2);
	darray_add(&primes, 3);
};

int is_prime(int x)
{
	int i, top, prime, lastprime;
	top = (int) sqrt(x);

	lastprime = primes.values[primes.used-1];
	if (x <= lastprime) {
		for (i = 0; i < primes.used; i++)
			if (primes.values[i] == x) return 1;
		return 0;
	};

	for (i = 0; i < primes.used; i++) {
		prime = primes.values[i];
		if (prime > top) return 1;
		if (x % prime == 0) return 0;
	};

	lastprime = prime;
	while (lastprime < top) {
		lastprime += 2;
		if (is_prime(lastprime)) {
			darray_add(&primes, lastprime);
		}
	};

	for (; i < primes.used; i++) {
		prime = primes.values[i];
		if (prime > top) return 1;
		if (x % prime == 0) return 0;
	};

	return 1;
};

struct st_btree_node {
	struct st_btree_node * left;
	struct st_btree_node * right;
	int value;
};

typedef struct st_btree_node * btree;

btree btree_new_node(int val)
{
	btree b = (struct st_btree_node *) malloc(sizeof(struct st_btree_node));
	b->left = NULL;
	b->right = NULL;
	b->value = val;
	return b;
};

void btree_del(btree root)
{
	if (root->left)
		btree_del(root->left);
	if (root->right)
		btree_del(root->right);
	free(root);
};

btree btree_add_left(btree root, int val)
{
	btree new = btree_new_node(val);
	root->left = new;
	return root->left;
};

btree btree_add_right(btree root, int val)
{
	btree new = btree_new_node(val);
	root->right = new;
	return root->right;
};

btree btree_sorted_add(btree root, int val)
{
	if (val <= root->value) {
		if (root->left == NULL)
			return btree_add_left(root, val);
		else
			return btree_sorted_add(root->left, val);
	} else {
		if (root->right == NULL)
			return btree_add_right(root, val);
		else
			return btree_sorted_add(root->right, val);
	}
};

int btree_check(btree root, int val)
{
	if (val == root->value)
		return 1;
	else if (val < root->value && root->left)
		return btree_check(root->left, val);
	else if (val > root->value && root->right)
		return btree_check(root->right, val);
	else
		return 0;
};

void btree_show(btree root, int indent)
{
	if (root == NULL) return;
	int i = indent;
	while (i--) putchar(' ');
	printf("%d\n", root->value);
	btree_show(root->left, indent + 2);
	btree_show(root->right, indent + 2);
};

void btree_plain_show(btree root)
{
	if (root->left)
		btree_plain_show(root->left);
	printf("%d ", root->value);
	if (root->right)
		btree_plain_show(root->right);
};

btree btree_unique_add(btree root, int val)
{
	if (val == root->value)
		return root;
	if (val < root->value) {
		if (root->left == NULL)
			return btree_add_left(root, val);
		else
			return btree_unique_add(root->left, val);
	} else {
		if (root->right == NULL)
			return btree_add_right(root, val);
		else
			return btree_unique_add(root->right, val);
	}
};

int btree_length(btree root)
{
	if (root == NULL)
		return 0;
	return btree_length(root->left) + btree_length(root->right) + 1;
};

