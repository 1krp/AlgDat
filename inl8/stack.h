#include "tree.h"
typedef struct stack_node {
    node* tree_node;
    struct stack_node* next;
} stack_node;

typedef struct stack {
    stack_node* top;
} stack;