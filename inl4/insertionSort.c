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

int insertionSort(int array[], int size) {
    for (int i = 0; i < size; i++) {
        int canditade = array[i];
        int j = i-1;
        while (j >= 0 && array[j] > canditade) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = canditade;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;
    printf("insertion Sort\n");

    int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000};

    for (int i = 0; i < 9; i++) {
        int size = sizes[i];
        long wall = LONG_MAX;
        int* unsorted = randomArray(size);

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        int found = insertionSort(unsorted, size);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        free(unsorted);
        printf("%d  %0.2f ns\n", size, (double)wall);
    }
}