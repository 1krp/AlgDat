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
} bucket;

typedef struct buckets {
    bucket *array;
    int n;
} buckets;



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

// Buckets

buckets *initialize_buckets(int num_buckets) {
    buckets *b = malloc(sizeof(buckets));  // Allocate memory for the buckets struct
    if (!b) return NULL;

    b->array = calloc(num_buckets, sizeof(bucket));  // Allocate array of buckets
    if (!b->array) {
        free(b);
        return NULL;
    }

    b->n = num_buckets;
    return b;
}
void print_buckets(buckets *b, int num) {
    for (int i = 0; i < num; i++) {
        printf("Bucket %d: ", i);
        cell *current = b->array[i].first;
        while (current) {
            printf("%d -> ", current->zip);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void populate_bucket(codes *postnr, buckets *bs, int mod) {
    for(int i = 0; i < postnr->n; i++) {
        int index = (postnr->areas[i].zip) % mod;

        // Allocate memory for new cell
        cell *new = malloc(sizeof(cell));
        if (!new) return;  // Check if malloc fails

        new->zip = postnr->areas[i].zip; // Assign zip code
        new->next = bs->array[index].first; // Link to the current first cell
        bs->array[index].first = new; // Update the bucket to point to new cell
    }
}

bool lookup_bucket(buckets *bs, int zip) {
    int index = zip % (bs->n);  // Get the bucket index
    cell *tmp = bs->array[index].first;  // Start at the first node in the bucket

    while (tmp != NULL) {  // Loop through the linked list
        if (tmp->zip == zip) {
            return true;  // Found the zip code, return true
        }
        tmp = tmp->next;  // Move to the next node
    }

    return false;  // Zip code not found
}



int main(int argc, char *argv[]) {
   /* if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    */
    codes *data = read_postcodes(argv[1]);
    printf("Number of dataentrys: %d\n",data->n);

    int mod = 1351;
    int zip = 11115;

    buckets *bs = initialize_buckets(mod);
    
    populate_bucket(data,bs,mod);
    print_buckets(bs,10);
    
    bool foudn = lookup_bucket(bs,zip);
    
    printf("lookup_bucket serched for zip: %d, result for lookup: %d \n",zip,foudn);
}
