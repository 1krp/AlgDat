#include <stdlib.h>
#include <stdio.h>

typedef struct stack {
    int top;
    int size;
    int *array;
} stack;

stack *new_stack(int size) {
    int *array = (int*)malloc(size*sizeof(int));
    stack *stk = (stack*)malloc(sizeof(stack));
    stk->top = 0;
    stk->array= array;
    stk->size = size;
    return stk;
}
void push(stack *stk, int val) {
    //:

    stk->array[stk->top] = val;
    stk->top += 1;


    /*
    if (stk->top == stk->size) {
    int size =
    stk->size =
    int *copy =     
    for (int i = 0; i < ...; i++) {
    copy[i] =
    }
    free(stk->array);
    stk->array = copy;
    }
    */

    
}
    int pop(stack *stk) {
    //:
    int val = stk->array[stk->top];
    stk->top -= 1;
    
    return val;
}

int main() {
    stack *stk = new_stack(10);
    push(stk, 32);
    push(stk, 33);
    push(stk, 34);
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
}