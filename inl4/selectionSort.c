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

int selectionSort(int array[], int size) {

    for (int i = 0; i < size -1; i++) {

    int candidate = array[i];

        for (int j = i; j < size ; j++) {
            if (array[j] < candidate) {
                candidate = array[j];
            }
        }
            int temp = array[i];
            array[i] = array[candidate];
            array[candidate] = temp;
    }
    return 0;
}
long bench(int size, int loop) {
    struct timespec t_start, t_stop;
    long time = 0;
    for (int i = 0; i < loop; i++) {
        int* unsorted = randomArray(size);
        long wall = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        selectionSort(unsorted, size);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        wall = nano_seconds(&t_start, &t_stop);   
        time += wall;
        free(unsorted);
    }
    return time/loop; 
}
int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;
    printf("Selection Sort\n");

    int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000};

    for (int i = 0; i < 9; i++) {
        int size = sizes[i];
        int loop = 1;
        long benchTime = bench(size, loop);

        printf("%d  %0.2f ns\n", size, (double)benchTime);
    }
}