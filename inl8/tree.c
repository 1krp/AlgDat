#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "tree.h"

tree *create_tree(){
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;

    return tr;
}

node *create_node(int value) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = value;
    nd->left = NULL;
    nd->right = NULL;

    return nd;
}

void free_node(node* nd){
    if (nd != NULL) {
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_tree(tree* tr) {
    if(tr != NULL) {
        free_node(tr);
    }
}
