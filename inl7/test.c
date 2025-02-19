#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void** items;  // Array of void* to store any type of data
    int front;     // Index of front element
    int rear;      // Index of last element
    int capacity;  // Maximum number of elements
    int size;      // Current number of elements
} Queue;

// Function prototypes
Queue* createQueue(int capacity);
int enqueue(Queue* queue, void* item);
void* dequeue(Queue* queue);
int isEmpty(Queue* queue);
int isFull(Queue* queue);
void freeQueue(Queue* queue);

int main() {
    // Create a queue with a capacity of 5
    Queue* q = createQueue(5);

    int a = 10, b = 20, c = 30;
    enqueue(q, &a);
    enqueue(q, &b);
    enqueue(q, &c);

    int* dequeued = (int*)dequeue(q);
    printf("Dequeued: %d\n", *dequeued); // Output: Dequeued: 10

    freeQueue(q);
    return 0;
}

// Create a queue with given capacity
Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) return NULL;

    queue->items = (void**)malloc(capacity * sizeof(void*));
    if (!queue->items) {
        free(queue);
        return NULL;
    }

    queue->capacity = capacity;
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;

    return queue;
}

// Add an item to the queue
int enqueue(Queue* queue, void* item) {
    if (isFull(queue)) return 0; // Queue is full

    queue->rear = (queue->rear + 1) % queue->capacity; // Circular increment
    queue->items[queue->rear] = item;
    queue->size++;

    return 1;
}

// Remove and return an item from the queue
void* dequeue(Queue* queue) {
    if (isEmpty(queue)) return NULL; // Queue is empty

    void* item = queue->items[queue->front];
    queue->front = (queue->front + 1) % queue->capacity; // Circular increment
    queue->size--;

    return item;
}

// Check if queue is empty
int isEmpty(Queue* queue) {
    return queue->size == 0;
}

// Check if queue is full
int isFull(Queue* queue) {
    return queue->size == queue->capacity;
}

// Free the queue memory
void freeQueue(Queue* queue) {
    free(queue->items);
    free(queue);
}
