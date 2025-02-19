#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "arrQueue.h"

queue* createQueue(int qSize) {
    queue* q = (queue*)malloc(sizeof(queue));   // malloc for q struct
    if (!q) return NULL;                        // If malloc fails

    q->items = (void**)malloc(qSize * sizeof(void*)); // malloc for q elements
    if (!q->items) {                            // If malloc fails
        free(q);
        return NULL;
    }

    q->qSize = qSize;
    q->first = 0;
    q->last = -1;
    q->top = 0;

    return q;
}

void freeQueue(queue* q) {
    free(q->items);
    free(q);
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
/*
void doubleQueue(queue** q) {
    queue* doubleQ = createQueue((*q)->qSize*2);

    // Copy elements from the old queue to the new one
    for (int i = 0; i < (*q)->top; i++) {
        int index = ((*q)->first + i) % (*q)->qSize;  // Keeps the correct order
        doubleQ->items[i] = (*q)->items[index];    // Copy the element
    }

    doubleQ->qSize = (*q)->qSize*2;
    doubleQ->first = 0;
    doubleQ->last = (*q)->top -1;
    doubleQ->top = (*q)->top;

    free((*q)->items);
    free(*q);

    *q = doubleQ;
}


void halfQueue(queue** q) {
    queue* halfQ = createQueue((*q)->qSize/2);

    // Copy elements from the old queue to the new one
    for (int i = 0; i < (*q)->top; i++) {
        int index = ((*q)->first + i) % (*q)->qSize;  // Keeps the correct order
        halfQ->items[i] = (*q)->items[index];    // Copy the element
    }

    halfQ->qSize = (*q)->qSize/2;
    halfQ->first = 0;
    halfQ->last = (*q)->top-1;
    halfQ->top = (*q)->top;

    free((*q)->items);
    free(*q);

    *q = halfQ;
}
*/
void enqueue(queue* q, void* item) {
    if (isFull(q)) {
        //doubleQueue(*q);
    } 

    q->last = (q->last +1) % q->qSize; // Circular logic
    q->items[q->last] = item;
    q->top++;
}
void* dequeue(queue* q) {
    if (isEmpty(q)) {
        printf ("error: queue is empty\n");
        return NULL;
    }

    void* item = q->items[q->first];
    q->first = (q->first + 1) % q->qSize;
    q->top--;

    if (isLessThanHalf(q)) {
        //halfQueue(*q);
    }

    return item;
}
void printQ(queue* q) {
    for (int i = 0; i < q->qSize; i++) {
        if (q->items[i] == NULL) {
            printf("Empty\n");
        } else {
            printf("%d\n", *(int*)q->items[i]);
        }
    }
}

int main(int argc, char const *argv[]) {
    queue* q = createQueue(10);

    int a = 10, b = 20, c = 30;
    enqueue(q, &a);
    enqueue(q, &b);

    printQ(q);

    int* dequeued = (int*)dequeue(q);
    printf("Dequeued: %d\n", *dequeued); 

    printQ(q);

    enqueue(q, 1);
    enqueue(q, 2);
    enqueue(q, 3);

    enqueue(q, 4);
    enqueue(q, 5);
    enqueue(q, 6);

    enqueue(q, 7);
    enqueue(q, 8);
    enqueue(q, 9);

    printQ(q);

    freeQueue(q);
    return 0;
    return 0;
}
