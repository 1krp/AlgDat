#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

/*
* Tree struct, node shared with queue
*/
typedef struct node {
    int value;
    struct node* left;
    struct node* right;
    struct node* next;
    
}node;

typedef struct tree{
    node* root;
}tree;

/*
* Queue struct
*/

typedef struct queue {
    node* first;
    node* last;
    node* sequenceNext;
} queue;

/*
* - - - - - Queue - - - - - - - - - - - - - - - - -
*/

// Creates a queue
queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    q->sequenceNext = NULL;
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
        q->sequenceNext = nd;
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
void init_tree2(tree *tr, int size) {

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

// Fills an array with unique numbers and shuffles them
void generate_shuffled_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;  // Unique values
    }

    // Fisher-Yates Shuffle
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Recursively inserts elements to create a balanced BST
void insert_balanced(tree* tr, int* arr, int start, int end) {
    if (start > end) return;

    int mid = (start + end) / 2;
    add_node(tr, arr[mid]);  // Add middle element first
    insert_balanced(tr, arr, start, mid - 1);
    insert_balanced(tr, arr, mid + 1, end);
}

// Initializes a more balanced tree
void init_tree(tree* tr, int size) {
    int* arr = (int*)malloc(size * sizeof(int));
    generate_shuffled_array(arr, size);
    insert_balanced(tr, arr, 0, size - 1);
    free(arr);
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
    // Bfs snurran
    while (q->first != NULL) {
        
        next = deque(q);    // Deque av noden
        printf("%d\n", next->value);

        if (next->left != NULL)     // Lägg till left-branch
            enque(q, next->left);
        if (next->right != NULL)    // Lägg till right-branch
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

node* bfs(node* nd, queue* q) {
    if (is_empty(q))  // Base case: Stop when queue is empty
        return NULL;
    
    node* next = deque(q); // Dequeue the next node
    printf("%d ", next->value); // Process the node

    // Enqueue children if they exist
    if (next->left != NULL)
        enque(q, next->left);
    if (next->right != NULL)
        enque(q, next->right);

    return next; // Return the dequeued node
}

void init_bfs(tree* tr, queue* q) {
    if (tr->root != NULL) {
        enque(q, tr->root); // Start with the root in the queue
        bfs(tr->root, q);
    }
}
/*
* - - - - Lasy sequence - - - - - - - - - - - - - - -
*/

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
    if (seq->q->sequenceNext == NULL) {
        printf("sequence empty\n");
        return 0;
    }else {
        node* next = seq->q->sequenceNext;
        // iterate sequenceNext 
        seq->q->sequenceNext = next->next;
        return next->value;
    }
}
void use_next(sequence* seq, int n) {
    for (int i = 0; i < n; i++){
        printf("%d \n", next(seq));
    }
    printf("Paused\n");
}

int main() {
    tree* tr = create_tree();
    init_tree(tr, 9);
    inorder_traversal(tr->root);
    printf("\n");
    print_tree(tr);
    printf("\n");
    print_breadth(tr);

    sequence* seq = create_sequence(tr);
    int n = 3;
    int next_s = 0;
    printf("\n");
    use_next(seq, 3);
    use_next(seq, 3);
}