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
    int loop = 1;
    printf("into search\n");

    for (int i = 0; i < loop; i++) {
        int key = keyArr[i];
    
        int first = 0;
        int last = size-1;
        
        
        while (first <= last) {
            // jump to the middle
            int index = first+(last-first)/2;
            printf("into3 search\n");
            if (array[index] == key) {
                break;
            }
            if (array[index] < key && index < last) {
                // what is the first possible page?
                first = index +1;
                
            }
            if (array[index] > key && index > first) {
                // what is the last possible page?
                last = index -1;
                
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



            int* keyArr = randomArray(1000);
            int* array = sortedArray(1000);
            
            long wall = binary_search(array,1000,keyArr);

            

            free(array);  
            free(keyArr);
        
            printf("%d  %0.2f ns\n", 100, (double)wall);
    
}