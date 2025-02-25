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

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
    (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}
long bench_lookup(int size, int loop) {
    struct timespec t_start, t_stop;
    tree *tr = create_tree();
    init_tree(tr, size);
    long time = 0;
    for (int i = 0; i < loop; i++) {
        long wall = LONG_MAX;
        int value = (rand() % size) + 1;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        lookup(tr, value);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
    }
    free_tree(tr);
    return time/loop;
}
int *bench_sizes(int size, int itr) {
    int* sizes = malloc(size * sizeof(int)); 
    int value = itr;   
    for (int i = 0; i < size; i++) {
        sizes[i] = value;
        value += itr; 
    }
    return sizes;
}

/*
int main(int argc, char const *argv[]) {
    tree* tr = create_tree();
    init_tree(tr, 10);
    return 0;
}
*/


int main(int argc, char const *argv[]) {
    printf("Binary tree lookup avrage \n");
    int* sizes = bench_sizes(20,100000);
    
    for (int i = 0; i < 20; i++) {
        int size = sizes[i];
        int loop = 500;
        long benchTime = bench_lookup(size, loop);

        printf("%d  %ld ns\n", size, benchTime);
    }
    
    free(sizes);
    return 0;
}
