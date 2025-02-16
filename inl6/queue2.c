#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "queuelist2.h"

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
    (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

/*

int empty(queue *q) {
    node *tmp = q->first;
    int value = tmp->value;
    free(tmp);
    return ;
}

*/
void queue_free(queue *q) {
    node *nxt = q->first;
    while (nxt != NULL) {
        node *tmp = nxt ->next;
        free(nxt);
        nxt = tmp;
    }
    free(q);
}

void enque(queue* q, int v) {
    node *new = (node*)malloc(sizeof(node));
    new->value = v;
    new->next = NULL;

    if (q->first == NULL) {
        q->first = new;
        q->last = new;
    } else{
        q->last->next = new;
        q->last = new;
    }
}

int dequeue(queue *q) {
    int res = 0;
    if (q->first != NULL) {
        node *tmp = q->first;
        res = tmp->value;
        q->first= tmp->next;
        free(tmp);
    }
    return res;
}

queue *queue_init(int size) {
    queue *a = create_queue();
    for (int i = 1; i <= size; i++) {
        enque(a, i);
    }
    return a;
}

void *queue_print(queue *lnk) {
    node *tmp = lnk->first;
    while (tmp != NULL) {
        printf("%d \n", tmp->value);
        tmp = tmp->next;        
    }
}

int *bench_sizes(int size, int itr) {
    int* sizes = malloc(size * sizeof(int)); 
    int value = itr;   
    for (int i = 0; i < size; i++) {
        sizes[i] = value;
        value += itr; 
    }
    return sizes;
}

long bench_b(int size, int loop) {
    struct timespec t_start, t_stop;
    long time = 0;
    queue *q = create_queue();
    
    for (int i = 0; i < loop; i++) {
        long wall = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        enque(q,size)
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        queue_free(q);
        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
    }
    queue_free(q);
    return time/loop;
}


int main(int argc, char *argv[]) {
    int* sizes = bench_sizes(100,1000);
    
    for (int i = 0; i < 76; i++) {
        int size = sizes[i];
        int loop = 500;
        long benchTime = bench_b(size, loop);

        printf("%d  %ld ns\n", size, benchTime);
    }
    
    free(sizes);
    return 0;
}

/*
int main(int argc, char *argv[]) {
    int size = 10;
    printf("Queue improved\n");
    queue *a = queue_init(size);
    queue_print(a);
    for (int i = 0; i < size; i++) {
    int d = dequeue(a);
    printf("dequed : %d \n",d);
    printf("Current state of queue:\n");
    queue_print(a);
    }
    free(a);
} 

*/