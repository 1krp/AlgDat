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

int *sortedArray(int size) {
    int *array = (int*)malloc(size * sizeof(int));
    int nxt = 0;

    for (int i = 0; i < size ; i++) {
        nxt += rand()%10 + 1;
        array[i] = nxt;
        }

    return array;
}

int* randomArray(int size) {
    int* randomArray = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
    {
       randomArray[i] = rand()%(2*size);
    }
    return randomArray;
    
}

long binary_search(int array[], int size, int keyArr[]) {
    struct timespec t_start, t_stop;
    int loop = 10000;
    
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) {
        int key = keyArr[i];
        int first = 0;
        int last = size-1;
        while (first <= last) {
            // jump to the middle
            int index = (first+last) / 2;
            if (array[index] == key) {  //Found
                break;
            }
            if (array[index] < key && index < last) {   //Right-half
                first = index + 1;
                continue;
            }
            if (array[index] > key && index > first) {  //Left-half
                last = index - 1;
                continue;
            }
            break;                                      //Does not exist
        }   
    }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);

    long wall = (nano_seconds(&t_start, &t_stop))/loop;
    return wall;
}


int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;

    printf("Binary\n");

    int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000,2048000,4096000,8192000,16384000};
    int k = 10;

    for (int i = 0; i < 15; i++)
    {
        int size = sizes[i];
        long min = LONG_MAX;
        
        for (int i = 0; i < k; i++)  {

            int* keyArr = randomArray(size);
            int* array = sortedArray(size);
            long wall = binary_search(array,size,keyArr);

            if (wall < min)
                min = wall;

            free(array);  
            free(keyArr);
        }
            printf("%d  %0.2f ns\n", size, (double)min);
    }
}