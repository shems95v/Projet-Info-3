#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include "usine.h"

typedef struct avl_node {
    Usine *usine;
    int height;
    struct avl_node *left;
    struct avl_node *right;
} AVLNode;

AVLNode* avl_insert(AVLNode *node, const char *id, AVLNode **found);
void avl_inorder_reverse(AVLNode *node, FILE *out, int mode);
void avl_free(AVLNode *node);

#endif
