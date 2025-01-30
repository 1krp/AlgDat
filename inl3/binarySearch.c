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

    for (int i = 0; i < size; i++)
    {
       randomArray[i] = rand()%(2*size);
    }
    return randomArray;
    
}

long binary_search(int array[], int size, int keyArr[]) {
    struct timespec t_start, t_stop;
    int loop = 1000;
    printf("into search\n");

    for (int i = 0; i < loop; i++) {
        int key = keyArr[i];
    
        int first = 0;
        int last = size-1;
        bool found = false;
        
        while (found == found) {
            // jump to the middle
            int index = (first+last)/2;

            if (array[index] == key) {
                found = true;
                break;
            }
            if (array[index] < key && index < last) {
                // what is the first possible page?
                first = index;
                continue;
            }
            if (array[index] > key && index > first) {
                // what is the last possible page?
                last = index ;
                continue;
            }
            // Why do we land here? What should we do?
            break;
        }
    }
    long wall = 11;
    return wall;
}


int main(int argc, char *argv[]) {
    struct timespec t_start, t_stop;

    printf("Binary\n");

    int sizes[] = {1000,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000};
    int k = 10;

    for (int i = 0; i < 11; i++)
    {
        int size = sizes[i];
        long min = LONG_MAX;
        
        for (int i = 0; i < k; i++)  {

            int* keyArr = randomArray(size);
            int* array = randomArray(size);
            
            long wall = binary_search(array,size,keyArr);

            if (wall < min)
                min = wall;
        }
            printf("%d  %0.2f ns\n", size, (double)min);
    }
}