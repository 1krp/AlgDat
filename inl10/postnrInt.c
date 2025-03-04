#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define AREAS 10000 // antalet rader/postnumr
#define BUFFER 200

typedef struct area {
    char *name;
    int zip;
    int pop;
} area;
typedef struct codes {
    area *areas;
    int n;
} codes;

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) +
    (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}


codes *read_postcodes(char *file) {

    codes *postnr = (codes*)malloc(sizeof(codes));
    area *areas = (area*)malloc(sizeof(area)*AREAS);
    FILE *fptr = fopen(file, "r");

    int k = 0;

    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;

    while ((k < AREAS) && (getline(&lineptr, &n, fptr) > 0)) {
        char *copy = strdup(lineptr);  // Duplicate line to avoid strtok issues
        if (!copy) break;

        area a;
        char *zip = strtok(copy, ",");
        a.zip = a.zip = atoi(zip)*100 + atoi(zip+3);

        char *name = strtok(NULL, ",");
        if (!name) {
            free(copy);
            continue;
        }
        a.name = strdup(name); // Proper allocation for name
        if (!a.name) {
            free(copy);
            continue;
        }

        char *pop = strtok(NULL, ",");
        if (!pop) {
            free(a.name);
            free(copy);
            continue;
        }
        a.pop = atoi(pop);

        areas[k++] = a;
        free(copy);
    }
    free(lineptr);
    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}

area *linear_search(codes *data, int zip) {

    for (int i = 0; i < data->n; i++) {
        if (data->areas[i].zip == zip) {
            return &data->areas[i];
        }
    }
    printf("not found\n");
    return NULL; 
}

area *binary_search(codes *data, int zip) {
    int left = 0, right = data->n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = data->areas[mid].zip;

        if (cmp == 0) {
            return &data->areas[mid]; // Found
        } else if (cmp < 0) {
            left = mid + 1; // Search right half
        } else {
            right = mid - 1; // Search left half
        }
    }
    return NULL; // Not found
}
long bench_linear(codes *data, int zip, int loop, int itr) {
    struct timespec t_start, t_stop;
    long min = LONG_MAX;
    for (int i = 0; i < loop; i++) {
        long time = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for(int j = 0; j < itr; j++) {
        linear_search(data, zip);
        }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        time = nano_seconds(&t_start, &t_stop);   
        if (time < min) {
            min = time;
        }
    }
    return min/itr;
}
long bench_binary(codes *data, int zip, int loop, int itr) {
    struct timespec t_start, t_stop;
    long min = LONG_MAX;
    for (int i = 0; i < loop; i++) {
        long time = LONG_MAX;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        for(int j = 0; j < itr; j++) {
            binary_search(data, zip);
            }
        clock_gettime(CLOCK_MONOTONIC, &t_stop);

        time = nano_seconds(&t_start, &t_stop);   
        if (time < min) {
            min = time;
        }
    }
    return min/itr;
}


int main(int argc, char *argv[]) {
   /* if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    */
    codes *data = read_postcodes(argv[1]);
    printf("Number of dataentrys: %d\n",data->n);
    printf("int as zip ");
    /*
    area* tmp = linear_search(data, "111 15");
    printf("was found: %s %s %d\n",tmp->zip, tmp->name, tmp->pop);

    tmp = binary_search(data, "999 15");
    if (tmp != NULL) {
    printf("was found: %s %s %d\n",tmp->zip, tmp->name, tmp->pop);
    }
    */
    printf("Linear bench\n");
    printf("Linear bench for: 111 15 -> takes: %ld ns\n",bench_linear(data,11115, 100,100));
    printf("Linear bench for: 984 99 -> takes: %ld ns\n",bench_linear(data,98499, 100,100));
    printf("\n");
    printf("Binary bench\n");
    printf("Binary bench for: 111 15 -> takes: %ld ns\n",bench_binary(data,11115, 100,100));
    printf("Binary bench for: 984 99 -> takes: %ld ns\n",bench_binary(data,98499, 100,100));
}