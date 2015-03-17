#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * List structure
 *
 * Single linked list structure
 */
struct st_list {
	int *value;
	int size;
};

typedef struct st_list *list;

list list_new(void)
{
	list l;
	l = malloc(sizeof(struct st_list));
	l->value = NULL;
	l->size = 0;
	return l;
}

void list_del(list list)
{
	if (list != NULL) {
		if (list->value != NULL)
			free(list->value);
		free(list);
	}
}

int list_pop(list list, int index)
{
	int value, i, j;
	if (index >= list->size) {
		printf("WOP! Accesing element %d of a %d list\n", index, list->size);
		exit(1);
	}
	value = list->value[index];
	i = 0;
	j = 0;
	if (index != list->size - 1) {
		for (i = 0; i < list->size; i++, j++) {
			if (j == index)
				j = j + 1;
			list->value[i] = list->value[j];
		}
	}
	list->size -= 1;
	return value;
}

int list_push(list list, int value)
{
	list->size += 1;
	list->value = realloc(list->value, list->size * sizeof(int));
	list->value[list->size - 1] = value;
}

void list_show(list list)
{
	int i;
	for (i = 0; i < list->size; i++)
		printf("%d ", list->value[i]);
	printf("\n");
}

/*
 * Dynamic array
 *
 * This array grows as elements are added
 */
struct st_dynamic_array {
	size_t alloc;
	size_t used;
	int * values;
};

typedef struct st_dynamic_array darray;

void darray_alloc(darray * a)
{
	a->alloc = 64;
	a->used = 0;
	a->values = malloc(a->alloc * sizeof(int));
};

void darray_free(darray * a)
{
	free(a->values);
};

void darray_add(darray * a, int val)
{
	if (a->used == a->alloc) {
		a->alloc *= 2;
		a->values = realloc(a->values, a->alloc * sizeof(int));
	}
	a->values[a->used++] = val;
};

/*
 * Binary tree
 *
 * Binary tree implementation
 */
struct st_btree_node {
	struct st_btree_node *left;
	struct st_btree_node *right;
	int value;
};

typedef struct st_btree_node *btree;

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

void btree_dotshow(btree root)
{
	if (root == NULL) {
		return;
	}
	printf("\"%p\" [label=%d, shape=circle, fixedsize=true]\n", root, root->value);
	btree_dotshow(root->left);
	btree_dotshow(root->right);
	if (root->left) {
		printf("\"%p\" -> \"%p\"\n", root, root->left);
	} else {
		printf("\"%p\" -> \"%p-left\"\n", root, root);
		printf("\"%p-left\" [label=\"/\", shape=point]\n", root);
	}
	if (root->right) {
		printf("\"%p\" -> \"%p\"\n", root, root->right);
	} else {
		printf("\"%p\" -> \"%p-right\"\n", root, root);
		printf("\"%p-right\" [label=\"/\", shape=point]\n", root);
	}
};

void btree_show(btree root)
{
	printf("digraph {\n");
	printf("node [fontname=sans]\n");
	btree_dotshow(root);
	printf("}\n");
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

/*
 * Set
 *
 * A set implemented using a binary tree.
 */
typedef btree set;

#define set_add btree_unique_add
#define set_new btree_new_node
#define set_show btree_show

int main(int argc, char *argv[])
{
	set s;

	s = set_new(5);
	set_add(s, 10);
	set_add(s, 2);
	set_add(s, 7);

	set_show(s);

	return 0;
};
