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

// The bucket data structure
typedef struct cell {
    int zip;
    struct cell *next;
} cell;

typedef struct bucket {
    cell *first;
    int hash;
} bucket;



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

/*
* - - - - - Hash - - - - - - - - - - 
*/
void collisions(codes *postnr, int mod) {
    int mx = 20;
    int indicator[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    int data[mod];
    int cols[mx];

    for(int i = 0; i < mod; i++) {
        data[i] = 0;
    }

    for(int i = 0; i < mx; i++) {
        cols[i] = 0;
    }

    for (int i = 0; i < postnr->n; i++) {
        int index = (postnr->areas[i].zip)%mod;
        data[index]++;
    }

    int sum = 0;

    for(int i = 0; i < mod; i++) {
        sum += data[i];
        if (data[i] < mx)
            cols[data[i]]++;
    }

    printf("mod: %d, number of zips: %d  \n", mod, sum);

    for (int i = 1; i < mx; i++) {
        if (cols[i] != 0) {
            printf("%d coll: %d,    ",indicator[i],cols[i]);
        }
    }
    printf("\n");
    int sumC = 0;
    for (int i = 1; i < mx; i++) {
        sumC += cols[i];
    }
    printf("sum of collisions (%d-%d): %d\n",indicator[1],indicator[mx-1],sumC);
    printf("\n");
}


int main(int argc, char *argv[]) {
   /* if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    */
    codes *data = read_postcodes(argv[1]);
    printf("Number of dataentrys: %d\n",data->n);
    printf("int as zip \n");
    


    int keyindex[100000];
    for (int i = 0; i < data->n; i++) {
        int index = data->areas[i].zip;
        keyindex[index] = data->areas[i].zip;   
    }


    collisions(data, 10000);
    collisions(data, 20000);
    collisions(data, 12345);
    collisions(data, 17389);
    collisions(data, 13513);
    collisions(data, 13600);
    collisions(data, 14000);
}
