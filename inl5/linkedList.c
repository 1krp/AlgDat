#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
    (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

linked *linked_create() {
linked *new = (linked*)malloc(sizeof(linked));
new->first = NULL;
return new;
}

void linked_free(linked *lnk) {
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}
/*
* Add the item as the first cell in the list
*/
void linked_add(linked *lnk, int item) {
    cell *new = (cell*)malloc(sizeof(cell));

    new -> value = item;            // set value to item
    new -> tail = lnk -> first;     // set taill to previous first
    lnk -> first = new;             // set linked first to point to the new cell
}

int linked_pop(linked *lnk) {
    cell *tmp = lnk->first;
    int cellValue = tmp -> value;
    lnk -> first = tmp -> tail;
    free(tmp);
    return cellValue;
}

/*
* Returns the length of the list
*/
int linked_length(linked *lnk) {
    int counter = 0;
    cell *tmp = lnk -> first;
    while (tmp != NULL) {
        counter++;
        tmp = tmp -> tail;
    }
    return counter;
}

/*
* Returns true/false depending on if the item is found in the list
*/
bool linked_find(linked *lnk, int item) {
    cell *tmp = lnk -> first;
    while (tmp != NULL) {
        if (tmp -> value == item) {
            return true;                    // If item is found, return true
        } else {
            tmp = tmp -> tail;
        }
    }
    return false;
}

/*
* Removes the item if it exists in the list and frees the cell
*/
void linked_remove(linked *lnk, int item){
    if (lnk -> first != NULL && lnk -> first -> value == item) {   // if first is item
       cell *del = lnk -> first;                                   // cell of item
       lnk -> first = lnk -> first -> tail;                        // skip aka delete
       free(del);                                                   
    }
    cell *tmp = lnk -> first;
    while (tmp != NULL && tmp -> tail !=NULL) {
        if (tmp-> tail -> value == item) {
            cell *del = tmp->tail;                                  // cell of item
            tmp -> tail = tmp -> tail -> tail;                      // skip aka delete
            free(del);
        } else {
            tmp = tmp ->tail;                                       // iterate
        }
    }   
}

/*
* Adds list b to the end of list a
*/
void linked_append(linked *a, linked *b) {
    cell *prv = a->first;
    while(prv->tail != NULL) {                      // finding end of list a 
        prv = prv -> tail;
    }
    cell *sprt = (cell*)malloc(sizeof(cell));       // 0 cell as sepearator
    sprt -> value = 0;
    sprt -> tail = b -> first;                      // appending    
    prv -> tail = sprt;                             // appending
    b->first = NULL;             
}

/*
* Generates a linked list
*/
linked *linked_init(int size) {
    linked *a = linked_create();
    for (int i = 1; i <= size; i++) {
        linked_add(a, i);
    }
    return a;
}

/*
* Print outs the values of the cells in a list
*/
void *linked_print(linked *lnk) {
    cell *tmp = lnk->first;
    while (tmp != NULL) {
        printf("%d \n", tmp->value);
        tmp = tmp->tail;        
    }
}

/*
* Creates a array of growing values
*/
int *bench_sizes(int size, int itr) {
    int* sizes = malloc(size * sizeof(int)); 
    int value = itr;   
    for (int i = 0; i < size; i++) {
        sizes[i] = value;
        value += itr; 
    }
    return sizes;
}
/*
* Function to use for benchmarks
*/
long bench_fixedB(int size, int loop) {
    struct timespec t_start, t_stop;
    long time = 0;
    for (int i = 0; i < loop; i++) {
        linked *a = linked_init(size);
        linked *b = linked_init(100);
        long wall = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        linked_append(a,b);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
        free(a);
    }
    return time/loop; 
}
/*
* Function to use for benchmarks
*/
long bench_fixedA(int size, int loop) {
    struct timespec t_start, t_stop;
    long time = 0;
    for (int i = 0; i < loop; i++) {
        linked *a = linked_init(10000);
        linked *b = linked_init(size);
        long wall = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        linked_append(a,b);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
        free(a);
    }
    return time/loop; 
}
/*

int main(int argc, char *argv[]) {
    linked *a = linked_init(10);
    linked *b = linked_init(10);
    linked_append(a,b);
    linked_print(a);

    int* sizes = bench_sizes(10,10);
    for (int i = 0; i < 10; i++) {
        printf("%d \n", sizes[i]);
    }
    
}

*/



int main(int argc, char *argv[]) {
    int* sizes = bench_sizes(100,1000);
    
    for (int i = 0; i < 76; i++) {
        int size = sizes[i];
        int loop = 50;
        long benchTime = bench_fixedA(size, loop);

        printf("%d  %ld ns\n", size, benchTime);
    }
    
    free(sizes);
    return 0;
}   

