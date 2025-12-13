#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

/* fonctions utilitaires */
static int max(int a, int b) { return a > b ? a : b; }
static int height(AVLNode *n) { return n ? n->height : 0; }

/* rotations */
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

static int balance(AVLNode *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* avl_insert(AVLNode *node, const char *id, AVLNode **found) {
    if (!node) {
        Usine *u = creer_usine(id);
        *found = malloc(sizeof(AVLNode));
        (*found)->usine = u;
        (*found)->left = (*found)->right = NULL;
        (*found)->height = 1;
        return *found;
    }

    int cmp = strcmp(id, node->usine->id);

    if (cmp == 0) {
        *found = node;
        return node;
    } else if (cmp < 0) {
        node->left = avl_insert(node->left, id, found);
    } else {
        node->right = avl_insert(node->right, id, found);
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int b = balance(node);

    if (b > 1 && strcmp(id, node->left->usine->id) < 0)
        return rotate_right(node);
    if (b < -1 && strcmp(id, node->right->usine->id) > 0)
        return rotate_left(node);
    if (b > 1 && strcmp(id, node->left->usine->id) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (b < -1 && strcmp(id, node->right->usine->id) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void avl_inorder_reverse(AVLNode *node, FILE *out, int mode) {
    if (!node) return;

    avl_inorder_reverse(node->right, out, mode);

    Usine *u = node->usine;
    if (mode == 0)
        fprintf(out, "%s;%.3f\n", u->id, u->vol_max / 1000.0);
    else if (mode == 1)
        fprintf(out, "%s;%.3f\n", u->id, u->vol_src / 1000.0);
    else
        fprintf(out, "%s;%.3f\n", u->id, u->vol_real / 1000.0);

    avl_inorder_reverse(node->left, out, mode);
}

void avl_free(AVLNode *node) {
    if (!node) return;
    avl_free(node->left);
    avl_free(node->right);
    liberer_usine(node->usine);
    free(node);
}
