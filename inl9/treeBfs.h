

/*
* Queue struct
*/

typedef struct queue {
    node *first;
    node *last;
} queue;


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
    node *root;
}tree;