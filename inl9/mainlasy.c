#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

// TREE

typedef struct node {
    int value;
    struct node* right;
    struct node* left;
    struct node* next;
} node;

typedef struct tree {
    node* root;
} tree;

node* construct_node(int value){
    node* nd = (node*)malloc(sizeof(node));
    nd->value = value;
    nd->left = NULL;
    nd->right = NULL;
    nd->next = NULL;
    return nd;
}

void free_node(node* nd){
    if (nd != NULL){
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_tree(tree* tr){
    if (tr != NULL)
        free_node(tr->root);
    free(tr);
}

int* get_sorted_array(int n) 
{
    int *array = (int*)malloc(n*sizeof(int));
    int nxt = 0;
    for (int i = 0; i < n ; i++) {
      nxt +=  rand()%3 + 1;
      array[i] = nxt;
    }
    return array;
}

node* build_tree(int start, int end, int* a) {
    if (start > end)
        return NULL;
    
    int mid = start + (end - start) / 2;
    node* root = construct_node(a[mid]);
    
    root->left  = build_tree(start, mid - 1, a);
    root->right = build_tree(mid + 1, end, a);
    
    return root;
}

tree* construct_tree(int n, int* a) {
    tree* t = malloc(sizeof(tree));
    t->root = build_tree(0, n - 1, a);
    return t;
}

// QUEUE

typedef struct queue {
    node* first;
    node* last;
} queue;

queue* create_queue() {
    queue* q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;

    return q;
}

void enque(queue* q, node* nd)
{
    if (q->last != NULL) {
        q->last->next = nd;
        q->last = nd;
    } else {
        q->first = nd;
        q->last = nd;
    }
}

node* deque(queue* q) {

    if (q->first == NULL) {
        printf("queue empty");
        return NULL;
    } else {
        node* temp = q->first;
        q->first = q->first->next;
        if (q->first == NULL) {
            q->last = NULL;
        }
        return temp;
    }
}

// LAZY SEQUENCE

typedef struct sequence {
    queue* q;
} sequence;

sequence* create_sequence (tree* tr){
    
    sequence* seq = (sequence*)malloc(sizeof(sequence));
    seq->q = create_queue();

    node* next = tr->root;
    queue* q = create_queue();

    // Köa root i q
    enque(q, next);

    while (q->first != NULL) {

        next = deque(q);
        // Köa första elementet från q till seq->q
        enque(seq->q, next);

        if (next != NULL) {
            if (next->left != NULL)
                enque(q, next->left);
            if (next->right != NULL)
                enque(q, next->right);
        }
    }

    return seq;
}

int next(sequence* seq) {
    return deque(seq->q)->value;
}

//PRINT

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

    sequence* seq = create_sequence(t);
    int n = 3;
    int next_s = 0;

    printf("\n");
    // Printa de n första elementen från trädet
    for (int i = 0; i < n; i++){
        next_s = next(seq);
        printf("%d \n", next_s);
    }
}