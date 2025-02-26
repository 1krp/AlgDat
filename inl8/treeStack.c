#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "stack.h"

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
/*
*
* Stack---------------------------------------------------------------------
*
*/

stack* create_stack() {
    stack* stk = (stack*)malloc(sizeof(stack));
    stk->top = NULL;
    return stk;
}

void push(stack* stk, node* tree_node) {
    stack_node* new_node = (stack_node*)malloc(sizeof(stack_node));
    new_node->tree_node = tree_node;
    new_node->next = stk->top;
    stk->top = new_node;
}

node* pop(stack* stk) {
    if (stk->top == NULL) return NULL;
    
    stack_node* temp = stk->top;
    node* tree_node = temp->tree_node;
    stk->top = stk->top->next;
    free(temp);
    return tree_node;
}

int is_empty(stack* stk) {
    return stk->top == NULL;
}

void free_stack(stack* stk) {
    while (!is_empty(stk)) {
        pop(stk);
    }
    free(stk);
}

void inorder_traversal(tree* tr) {
    stack* stk = create_stack();
    node* cur = tr->root;
    
    while (cur != NULL || stk->top != NULL) {

        // Move to the leftmost node
        while (cur != NULL) {
            push(stk, cur);
            cur = cur->left;
        }
        
        // Pop node from the stack, process it, and go right
        cur = pop(stk);
        printf("%d ", cur->value);
        
        // Now move to the right subtree
        cur = cur->right;
    }

    free_stack(stk);
}

/*
* 
* Stack---------------------------------------------------------------------
*
*/

int main() {
    tree* tr = create_tree();
    
    init_tree(tr, 15);

    printf("Inorder Traversal using Explicit Stack:\n");
    inorder_traversal(tr);
    printf("\n");
    printf("\n");
    printf("\n");
    print_tree(tr);
    free_tree(tr);
    return 0;
}

/*
int main(int argc, char const *argv[]) {
    tree* tr = create_tree();
    init_tree(tr, 30);
    printf("root: %d \n",tr->root->value);
    print_tree(tr);
    free_tree(tr);
    return 0;
}
*/


