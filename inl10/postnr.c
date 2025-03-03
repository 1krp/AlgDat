#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define AREAS 1000 // antalet rader/postnumr
#define BUFFER 200

typedef struct area {
    char *name;
    char *zip;
    int pop;
} area;
typedef struct codes {
    area *areas;
    int n;
} codes;


codes *read_postcodes(char *file) {

    codes *postnr = (codes*)malloc(sizeof(codes));
    area *areas = (area*)malloc(sizeof(area)*AREAS);
    FILE *fptr = fopen(file, "r");

    int k = 0;

    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;

    while((k < AREAS) && (getline(&lineptr, &n, fptr) > 0)) {
        char *copy = (char*)malloc(sizeof(char)*n);
        strcpy(copy, lineptr);

        area a;
        a.zip = strtok(copy, ",");
        a.name = strtok(NULL, ",");
        a.pop = atoi(strtok(NULL, ","));

        areas[k++] = a;
    }

    fclose(fptr);
    postnr->areas = areas;
    postnr->n = k;
    return postnr;
}