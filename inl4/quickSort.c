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

int* randomArray(int size) {
    int* randomArray = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
       randomArray[i] = rand()%(2*size);
    }
    return randomArray; 
}

void quickSort(int *org, int start, int end) {
    if (end <= start) {
        return;
    }                   //base case
    int pivot = partition(org, start, end);
    quickSort(org, start, pivot - 1);
    quickSort(org, pivot + 1, end);
}

int partition(int *org, int start, int end) {
    int pivot = org[end];
    int i = start -1;
    
    for (int j = start; j <= end -1; j++) {
        if(org[j] < pivot) {
            i++;
            int temp = org[i];
            org[i] = org[j];
            org[j] = temp;
        
        }
    }
    i++;
    int temp = org[i];
    org[i] = org[end];
    org[end] = temp;
    return i;
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;
    printf("Quick sort\n");
    int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000};
    
    for (int i = 0; i < 11; i++) {
        int size = sizes[i];
        long wall = LONG_MAX;
        int* unsorted = randomArray(size);
        int loop = 1;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for (int i = 0; i < loop; i++) {
            quickSort(unsorted, 0, size - 1);
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = (nano_seconds(&t_start, &t_stop))/loop;   
        free(unsorted);
       
        printf("%d  %0.2f ns\n", size, (double)wall);
    }
}