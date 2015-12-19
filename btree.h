
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

