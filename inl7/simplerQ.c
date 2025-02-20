#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "simplerQ.h"


queue* createQ(int qSize) {
    queue* q = (queue*)malloc(sizeof(queue));       // mallocs q struct
    q->items = (int*)malloc(qSize * sizeof(int));   // mallocs array

    q->qSize = qSize;
    q->first = 0;  
    q->last = -1;  
    q->top = 0;    

    return q;
}

bool isFull(queue* q) {
    bool full = false;
    if (q->top == q->qSize) {
        full = true;
    }
    return full;
}
bool isEmpty(queue* q) {
    bool empty = false;
    if (q->top == 0) {
        empty = true;
    }
    return empty;
}
bool isLessThanHalf(queue* q) {
    bool itIs = false;
    if (q->top < (q->qSize)/2 && q->qSize > 10) {
        itIs = true;
    }
    return itIs;
}

queue* doubleQsize(queue* q) {
    int* doubleQ = (int*)malloc((q->qSize*2) * sizeof(int));
    for (int i = 0; i < q->top; i++) {
        int index = (q->first + i) % q->qSize;
        doubleQ[i] = q->items[index];
    }
    
    free(q->items);
    q->items = doubleQ;
    q->qSize *= 2;
    q->first = 0;
    q->last = q->top - 1;
    
    return q;
}

queue* halfQsize(queue* q) {
    int* halfQ = (int*)malloc((q->qSize/2) * sizeof(int));
    for (int i = 0; i < q->top; i++) {
        int index = (q->first + i) % q->qSize;
        halfQ[i] = q->items[index];
    }
    
    free(q->items);
    q->items = halfQ;
    q->qSize /= 2;
    q->first = 0;
    q->last = q->top - 1;
    
    return q;
}

queue* resizeQ(queue* q,double operator) {
    int* newArr = (int*)malloc((q->qSize*operator) * sizeof(int));

    for (int i = 0; i < q->top; i++) {
        int index = (q->first + i) % q->qSize;
        newArr[i] = q->items[index];
    }
    
    free(q->items);
    q->items = newArr;
    q->qSize *= operator;
    q->first = 0;
    q->last = q->top - 1;
    
    return q;
    
}

void enqueue(queue* q, int item) {
    if (isFull(q)) {
        printf("Queue is full, doubling Q size \n");
        //q = doubleQsize(q);
        q = resizeQ(q, 2);
    } 

    q->last = (q->last + 1) % q->qSize; // Circular logic
    q->items[q->last] = item;
    q->top++;
}
int dequeue(queue* q) {
    if (isEmpty(q)) {
        printf ("error: queue is empty\n");
        exit(1);
    }
    int item = q->items[q->first];
    q->first = (q->first + 1) % q->qSize;
    q->top--;
    
    if (isLessThanHalf(q)) {
        //q = halfQsize(q);
        q = resizeQ(q, 0.5);
    }

    return item;
}

void printQ(queue* q) {
    for (int i = q->first; i <= q->last; i++) {
            printf("%d\n",q->items[i]);
    }
}

int main(int argc, char const *argv[]) {
    queue* q = createQ(10);
    printf("Queue created with size: %d\n", q->qSize);

    for (int i = 1; i <= 45; i++) {
        enqueue(q, i * 10);
    }

    printQ(q);

    for (int i = 0; i <= 40; i++) {
        printf("Dequed: %d\n",dequeue(q));
    }

    printQ(q);
    printf("%d %d\n",q->qSize,q->top);
    free(q->items);
    free(q);
    return 0;
}
