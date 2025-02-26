#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "treeBfs.h"


/*
* - - - - - Queue - - - - - - - - - - - - - - - - -
*/

// Creates a queue
queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

// Checks if the queue is empty
bool empty(queue *q) {
    bool empty = false;
    if (q->first == NULL) {
        empty = true;
    }
    return empty;   
}

// Frees all the nodes in the queue and the queue itself
void queue_free(queue *q) {
    node *nxt = q->first;
    while (nxt != NULL) {
        node *tmp = nxt ->next;
        free(nxt);
        nxt = tmp;
    }
    free(q);
}

// Enqueues a node to the queue
void enque(queue* q, node* nd) {
    if (q->last != NULL) {
        q->last->next = nd;
        q->last = nd;
    } else {
        q->first = nd;
        q->last = nd;
    }
}

// Deques a node from the queue
node* deque(queue* q) {

    if (q->first == NULL) {
        printf("queue is empty");
        exit(1);
    } else {
        node* temp = q->first;
        q->first = q->first->next;
        if (q->first == NULL) {
            q->last = NULL;
        }
        return temp;
    }
}

/*
* - - - - - Tree - - - - - - - - - - - - - - - - -
*/

// Creates a new Tree
tree *create_tree(){
    tree *tr = (tree*)malloc(sizeof(tree));
    tr->root = NULL;

    return tr;
}

// Creates a new node
node *create_node(int value) {
    node *nd = (node*)malloc(sizeof(node));
    nd->value = value;
    nd->left = NULL;
    nd->right = NULL;
    nd->next = NULL;

    return nd;
}

// Frees the nodes, used by Free_tree
void free_node(node* nd){
    if (nd != NULL) {
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

// Frees nodes and the tree
void free_tree(tree* tr) {
    if(tr != NULL) {            
        free_node(tr->root);        // Frees nodes in tree
        free(tr);                   // Frees tree
    }
}

// Togeter with add_node as wrapper, adds nodes to the tree
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

// Wrapper for add_node_recusive, initilises where to start
void add_node(tree* tr, int value) {
    tr->root = add_node_recursive(tr->root, value);
}

// Creates a tree from a empty tree, with a given size
void init_tree(tree *tr, int size) {

    for (int i = 0; i < size; i++) {
        int value = rand() % (size*2); 
        add_node(tr, value);
    }
}

// Prints nodes in a tree like structure, the leftmost node is the root
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

// Wrapper for print_node
void print_tree(tree *tr) {
    if(tr->root !=NULL) {
        print_node(tr->root,0);
    }
}

/*
* - - - - - Breadth fisrt search - - - - - - - - - - - - - -
*/

//
void print_breadth(tree* tr) {
    node* next = tr->root;
    queue* q = create_queue();

    // Enqueue root
    enque(q, next);

    while (q->first != NULL) {
        // Print first from root
        next = deque(q);
        printf("%d\n", next->value);

        // Lägg till left och right
        if (next->left != NULL)
            enque(q, next->left);
        if (next->right != NULL)
            enque(q, next->right);
    }
}

void inorder_traversal(node* nd) {
    if (nd != NULL) {
        inorder_traversal(nd->left);
        printf("%d ", nd->value);
        inorder_traversal(nd->right);
    }
}

int main() {

    srand(time(NULL));
    int* a = get_sorted_array(16);
    tree* t = construct_tree(16, a);
    inorder_traversal(t->root);
    printf("\n");
    print_breadth(t);
}