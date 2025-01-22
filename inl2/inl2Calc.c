#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
        stk->array = copy;                          //frees up the old array
    }                                              
    stk->array[stk->top] = val;
    stk->top += 1;

}
    int pop(stack *stk) {

    if (stk->top <= stk->size /4) {
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



int main() {
    stack *stk = new_stack(4);
    printf("HP-35 pocket calculator\n");

    size_t n = 10;
    char buff[n];
    char *buffer = buff;
    bool run = true;

    while(run) {
        printf(" > ");
        getline(&buffer, &n, stdin);
    if (strcmp(buffer, "\n") == 0) {
    run = false;

    } else if (strcmp(buffer, "+\n") == 0) {    //Addition
        int a = pop(stk);
        int b = pop(stk);
        push(stk, a+b);

    } else if (strcmp(buffer,"-\n") == 0) {     //Subtraction
        int a = pop(stk);
        int b = pop(stk);
        push(stk, b-a);

    } else if (strcmp(buffer,"*\n") == 0) {     //Multiplication
        int a = pop(stk);
        int b = pop(stk);
        push(stk, a*b);
    
    } else {
        buffer[strcspn(buffer, "\n")] = '\0';                                    
        int val = atoi(buffer);
        push(stk, val);
    }
    }

    printf("the result is: %d\n\n", pop(stk));
    printf("I kinda hate reversed polish notation, don't you?\n");

}