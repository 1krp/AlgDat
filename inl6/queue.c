#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include "queuelist.h"

queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    q->first = NULL;
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

void enque(queue* q, int v) {
    node *new = (node*)malloc(sizeof(node));
    new->value = v;
    new->next = NULL;

    if (q->first == NULL) {
        q->first = new;
    } else{
        node *tmp = q->first;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new;
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
int main(int argc, char *argv[]) {
    int size = 10;
    printf("Queue\n");
    queue *a = queue_init(size);
    queue_print(a);
    for (int i = 0; i < size; i++) {
    int d = dequeue(a);
    printf("dequed : %d \n",d);
    printf("Current state of queue:\n");
    queue_print(a);
    }
} 