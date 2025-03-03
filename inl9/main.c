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

void add(tree* tr, int value){
    node* curr = tr->root;
    while(true)
    {
        if (curr->value == value){
            printf("Value already in tree");
            return;
        } else if (value < curr->value){
            if (curr->left != NULL) {
                curr = curr->left;
            }
            else {
                curr->left = construct_node(value);
                return;
            }
        } else {
            if (curr->right != NULL) {
                curr = curr->right;
            }
            else {
                curr->right = construct_node(value);
                return;
            }
        }
    }
}

void add_re(node* root, int value){

    // Sätt rot-nod till current
    node* curr = root;

    if (curr->value == value){
        printf("Value already in tree");
        return;
    } else if (value < curr->value){
        if (curr->left != NULL) {
            add_re(curr->left, value);
            return;
        }
        else {
            curr->left = construct_node(value);
            return;
        }
    } else {
        if (curr->right != NULL) {
            add_re(curr->right, value);
            return;
        }
        else {
            curr->right = construct_node(value);
            return;
        }
    }
}

bool lookup(node* root, int value) {
    if (root == NULL)
        return false;
    
    if (root->value == value)
        return true;
    else if (value < root->value)
        return lookup(root->left, value);
    else
        return lookup(root->right, value);
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

// Function to create a new tree with n nodes.
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

int main() {

    srand(time(NULL));
    int* a = get_sorted_array(16);
    tree* t = construct_tree(16, a);
    inorder_traversal(t->root);
    printf("\n");
    print_tree(t);
    printf("\n");
    print_breadth(t);
}