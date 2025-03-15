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
            printf("%d coll: %d\n",indicator[i],cols[i]);
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

int *arr_bucket(codes *postnr, int mod) {
    int *arr = (int *)calloc(mod*8, sizeof(int)); // Allocate enough space
    if (!arr) return NULL; 
    // Increment number of collisions
    for (int i = 0; i < postnr->n; i++) {
        int index = (postnr->areas[i].zip)%mod; 
        arr[index]++;  
    }
    // Find a empty spot for the zip, after the index
    for (int i = 0; i < postnr->n; i++) {
        int index = (postnr->areas[i].zip)%mod;
        int pos = index + arr[index]; 
        while (arr[pos] != 0) {
            pos++;
        }
        arr[pos] = postnr->areas[i].zip; // Store zip
    }
    return arr;
}
int map_lookup(int *map, int mod, int zip) {
    int index = (zip%mod);
    if (map[index] > 10000 || map[index] == 0) {
        return 0;
    }
    for (int i = index; i < (mod * 3); i++) {
        if (map[i] == zip) {
            return i - index;
        }
        if (map[i] == 0) {
            return 0;
        } 
    }
    return 0; 
}
int distande_lookup(int *mods, codes *data) {
    printf("start \n");
    for(int i = 0; i < 5; i++) {
        
        int counter = 0;
        int mod = mods[i];
        int * map = arr_bucket(data,mod);

        for(int j = 0; j < data->n; j++) {
            counter += map_lookup(map,mod,data->areas[j].zip);
        }
        counter = counter/(data->n);
        printf("For mod = %d -> avrage traversal distance -> %d\n", mod, counter);
    }
}


int main(int argc, char *argv[]) {
    codes *data = read_postcodes(argv[1]);
    printf("Number of dataentrys: %d\n",data->n);


    int *map = arr_bucket(data,41031);
    
    for(int i = 11115; i < 11115+20; i++) {
        printf("index: %d -> %d \n",i,map[i]);
    }
    
    int zip = 52132;
    int foudn = map_lookup(map, 41031, zip);

    int hash = zip%41031;

    printf("int hash: %d \n", hash);

    printf("map_lookup serched for zip: %d, result for lookup: %d \n",zip,foudn);

    printf("\n");
    printf("\n");

    int mods[] = {123450, 15351, 21031, 41031, 92413};

    distande_lookup(mods,data);

    free(map);
}
