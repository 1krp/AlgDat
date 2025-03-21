#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
    (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

int* randomArray(int size) {
    int* randomArray = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
       randomArray[i] = rand()%(2*size);
    }
    return randomArray; 
}

void mergeSort(int *org, int *aux, int lo, int hi) {
    if (lo != hi) {
    int mid = (lo + hi)/2;

    mergeSort(org, aux, lo, mid);
    mergeSort(org, aux, mid+1, hi);

    merge(org, aux, lo, mid, hi);
    }
}
void merge(int *org, int *aux, int lo, int mid, int hi) {
    for (int i = lo; i <= hi; i++) {
        aux[i] = org[i];
    }
    int i = lo;
    int j = mid+1;
    for (int k = lo; k <= hi; k++) {
        if (i > mid) { 
            org[k] = aux[j++];
        } else if (j > hi) { 
            org[k] = aux[i++];
        } else if (aux[i] <= aux[j]) { 
            org[k] = aux[i++];
        } else {
            org[k] = aux[j++];
        }
    }
}
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
long bench(int size, int loop) {
    struct timespec t_start, t_stop;
    long time = 0;
    for (int i = 0; i < loop; i++) {
        int* unsorted = randomArray(size);
        int* aux = malloc(size * sizeof(int));
        long wall = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        mergeSort(unsorted, aux, 0, size - 1);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
        free(unsorted);
        free(aux);
    }
    return time/loop;
}
int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;
    printf("Merge sort\n");
    int sizes[250];
    int sum = 1000;
    for (int i = 0; i < 250; i++) {
        if (i<100) {
            sum += 1000;
            sizes[i] = sum;
        } else {
            sum += 10000;
            sizes[i] = sum;
        }
    }
    

    //int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000, 2048000};
    
    
    for (int i = 0; i < 250; i++) {
        int size = sizes[i];
        int loop = 1;
        long benchTime = bench(size, loop);
        long bechSize = benchTime/size;
        long benchLog = (benchTime/size)*(log2l(size));
       
        printf("%d  %ld ns\n", size, benchTime);
    }
}