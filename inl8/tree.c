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
        free_node(tr->root);        // Frees nodes in tree
        free(tr);                   // Frees tree
    }
}

node* add_node_recursive(node* nd, int value) {
    if (nd == NULL) {                       // "Base case"
        return create_node(value);
    }
    if (nd->value == value) {
        return nd;
    }
    
    if (value < nd->value) {
        nd->left = add_node_recursive(nd->left, value);
    } else {
        nd->right = add_node_recursive(nd->right, value);
    }
    return nd;
}

void add_node(tree* tr, int value) {
    tr->root = add_node_recursive(tr->root, value);
}

void init_tree(tree *tr, int size) {

    for (int i = 0; i < size; i++) {
        int value = rand() % (size*2); 
        add_node(tr, value);
    }
}

bool lookup_recursive(node* nd, int value) {
    if (nd == NULL) {                       // "Base case"
        return false;
    }
    if(nd->value == value) {                // "Base case ish"
        return true;
    }
    if (value < nd->value) {
        return lookup_recursive(nd->left, value);
    } else {
        return lookup_recursive(nd->right, value);
    }
    return nd;
}
bool lookup(tree *tr, int value) {
    return lookup_recursive(tr->root, value);
}
void print_node(node *nd, int depth) {
    if (nd != NULL) {
        print_node(nd->left,depth +1);
        for (int i = 0; i < depth; i++) {
            printf("    ");  
        }
        printf("%d \n",nd->value);
        print_node(nd->right, depth +1);
    } 
}
void print_tree(tree *tr) {
    if(tr->root !=NULL) {
        print_node(tr->root,0);
    }
}


int main(int argc, char const *argv[]) {
    tree* tr = create_tree();
    init_tree(tr, 30);
    printf("root: %d \n",tr->root->value);
    print_tree(tr);
    free_tree(tr);
    return 0;
}



