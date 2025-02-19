typedef struct{
    void** items;   // Array of void* to store any type of data
    int first;     // Index of front element
    int last;      // Index of last element
    int qSize;  // Maximum number of elements
    int top;      // Current number of elements
} queue;

queue* createQueue(int capacity);
void enqueue(queue* q, void* item);
void* dequeue(queue* q);
bool isEmpty(queue* q);
bool isFull(queue* q);
bool isLessThanHalf(queue* q);
void freeQueue(queue* q);
void doubleQueue(queue** q);
void halfQueue(queue** q);
