#include "btree.h"

btree btree_new_node(TREE_TYPE val)
{
	btree b = (struct st_btree_node *) malloc(sizeof(struct st_btree_node));
	b->left = NULL;
	b->right = NULL;
	b->value = val;
	return b;
}

void btree_destroy(btree root)
{
	if (root->left)
		btree_destroy(root->left);
	if (root->right)
		btree_destroy(root->right);
	free(root);
}

btree btree_add_left(btree root, TREE_TYPE val)
{
	btree new = btree_new_node(val);
	root->left = new;
	return root->left;
}

btree btree_add_right(btree root, TREE_TYPE val)
{
	btree new = btree_new_node(val);
	root->right = new;
	return root->right;
}

btree btree_sorted_add(btree root, TREE_TYPE val)
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
}

int btree_check(btree root, TREE_TYPE val)
{
	if (val == root->value)
		return 1;
	else if (val < root->value && root->left)
		return btree_check(root->left, val);
	else if (val > root->value && root->right)
		return btree_check(root->right, val);
	else
		return 0;
}

void btree_dotshow(btree root)
{
	if (root == NULL) {
		return;
	}
	printf("\"%p\" [label=%d, shape=circle, fixedsize=true]\n", (void*)root, root->value);
	btree_dotshow(root->left);
	if (root->left) {
		printf("\"%p\" -> \"%p\"\n", (void*) root, (void*) root->left);
	} else {
		printf("\"%p\" -> \"%p-left\"\n", (void*)root, (void*)root);
		printf("\"%p-left\" [label=\"/\", shape=point]\n", (void*)root);
	}
	btree_dotshow(root->right);
	if (root->right) {
		printf("\"%p\" -> \"%p\"\n", (void*)root, (void*)root->right);
	} else {
		printf("\"%p\" -> \"%p-right\"\n", (void*)root, (void*)root);
		printf("\"%p-right\" [label=\"/\", shape=point]\n", (void*)root);
	}
}

void btree_show(btree root)
{
	printf("digraph {\n");
	printf("node [fontname=sans]\n");
	btree_dotshow(root);
	printf("}\n");
}

void btree_plain_show(btree root)
{
	if (root->left)
		btree_plain_show(root->left);
	printf("%d ", root->value);
	if (root->right)
		btree_plain_show(root->right);
}

btree btree_unique_add(btree root, TREE_TYPE val)
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
}

int btree_length(btree root)
{
	if (root == NULL)
		return 0;
	return btree_length(root->left) + btree_length(root->right) + 1;
}

