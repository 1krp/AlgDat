typedef struct {
    int* items;   // Array of ints
    int first;     // Index of front element
    int last;      // Index of last element
    int qSize;  // Maximum number of elements
    int top;      // Current number of elements
} queue;