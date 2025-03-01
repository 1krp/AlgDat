#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

    if (stk->top == stk->size) {
        int size = stk->size *2;
        stk->size = size;
        int *copy = (int*)malloc(size*sizeof(int)); //makes a new array double in size

        for (int i = 0; i < stk->top; i++) {
            copy[i] = stk->array[i];
        }                                           //copys over the old elements

        free(stk->array);
        stk->array = copy;
    }                                               //frees up the old array

    stk->array[stk->top] = val;
    stk->top += 1;

}
    int pop(stack *stk) {

    if (stk->top == stk->size /4)
    {
        int size = stk->size /2;
        stk->size = size;
        int *copy = (int*)malloc(size * sizeof(int));

        for (int i = 0; i < stk->top; i++) {
            copy[i] = stk->array[i];
        }                                           //copys over the old elements

        free(stk->array);
        stk->array = copy;
        
    }
    stk->top -= 1;
    int val = stk->array[stk->top];
    
    return val;
}

/*
int main() {
    stack *stk = new_stack(4);
    push(stk, 32);
    push(stk, 33);
    push(stk, 34);
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
    printf("pop : %d\n", pop(stk));
}
*/


int main() {
    stack *stk = new_stack(4);
    int n = 15;
    for(int i = 0; i < n; i++) {
        push(stk, i+30);
    }
    for(int i = 0; i < stk->top; i++) {
        printf("stack[%d] : %d\n", i, stk->array[i]);
    }
    int val = pop(stk);
    while(val != 0) { // assuming 0 is returned when the stack is empty
        printf("pop : %d\n", val);
        val = pop(stk);
    }
}


