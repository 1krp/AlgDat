#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

#define CONNECTIONS 120
#define BUFFER 200
#define MOD 61

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec)*1000000000;
}

// Id till städer
int next_city_id = 1;

struct city;

typedef struct connection {
    struct city* city;
    int dist;
    struct connection* next;
} connection;

typedef struct city {
    char* name;
    int id;
    connection* connections;
    struct city* next;
} city;

typedef struct bucket {
    city* first;
} bucket;

typedef struct map {
    bucket** cities;
} map;

// Hashfunktion som skapar index av stadens namn
int hash(char *name, int mod) {
    int h = 0;
    int i = 0;
    unsigned char c = 0;
    while((c = name[i]) != 0) {
        h = (h*31 + c) % mod;
        i++;
    }
    return h;
}

void connect(city* src, city* dst, int dist) {

    // Skapa ny connection a->b
    connection* newConnection = (connection*)malloc(sizeof(connection));
    newConnection->city = dst;
    newConnection->dist = dist;
    newConnection->next = NULL;

    // Sök och lägg till i src

    connection* current = src->connections;
    connection* last = NULL;

    while (current != NULL) {
        // Om staden redan finns i listan
        if (current->city->id == dst->id) {
            return;
        }
        last = current;
        current = current->next;
    }
    // last now points to the last node; append the new connection.
    if (last != NULL) {
        last->next = newConnection;
    } else {
        // This case is handled separately, but it's good to be clear.
        src->connections = newConnection;
    }
}

city* lookup(bucket** table, char* name) {

    // Compute the hash index for the given city name.
    int index = hash(name, MOD);
    bucket* b = table[index];
    
    // If no bucket exists at this index yet, allocate one.
    if (b == NULL) {
        b = (bucket*)malloc(sizeof(bucket));
        if (b == NULL) {
            perror("Failed to allocate memory for new bucket");
            exit(EXIT_FAILURE);
        }
        b->first = NULL;
        table[index] = b;
    }
    
    // Search through the bucket's linked list.
    city* current = b->first;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // City already exists; return it.
            return current;
        }
        current = current->next;
    }
    
    // City not found; create a new one.
    city* new_city = (city*)malloc(sizeof(city));
    if (new_city == NULL) {
        perror("Failed to allocate memory for new city");
        exit(EXIT_FAILURE);
    }
    
    new_city->name = strdup(name);
    if (new_city->name == NULL) {
        perror("Failed to allocate memory for city name");
        exit(EXIT_FAILURE);
    }
    
    new_city->id = next_city_id++;   // Assign a unique ID
    new_city->connections = NULL;     // No connections yet
    
    // Insert new city at the head of the bucket's linked list.
    new_city->next = b->first;
    b->first = new_city;
    
    return new_city;
}

map* graph(char* file) {

    // Skapa bucketarray (map)
    bucket** cities = malloc(sizeof(bucket*) * MOD);

    // Sätt alla pointers till NULL
    for (int i = 0; i < MOD; i++) {
        cities[i] = NULL;
    }

    // buckets or open adressing
    map* trains = (map*)malloc(sizeof(map));
    trains->cities = cities;

    // Open a file in read mode
    FILE *fptr = fopen(file, "r");
    if (fptr == NULL)
        return NULL;

    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;
    while(getline(&lineptr, &n, fptr) > 0) {
        char *copy = (char*)malloc(sizeof(char)*(strlen(lineptr)+1));
        strcpy(copy, lineptr);
        // divide the copy into the three parts
        city *src = lookup(cities, strtok(copy, ","));
        city *dst = lookup(cities, strtok(NULL, ","));
        int dist = atoi(strtok(NULL, ","));

        // add the connection
        connect(src, dst, dist);
        connect(dst, src, dist);
    }

    fclose(fptr);
    return trains;
}

// SEARCH 1 - max limit

int shortest(city* from, city* to, int left) {
    if (from == to) {
        return 0;
    }
    
    int sofar = -1;
    connection* nxt = from->connections;
    while(nxt != NULL) {
        if (nxt->dist <= left) {
            int d = shortest(nxt->city, to, left - nxt->dist);
            if (d >= 0 && ((sofar == -1 ) || (d + nxt->dist) < sofar))
                sofar = (d + nxt->dist);
        }
        nxt = nxt->next;
    }
    return sofar;
}

// SEARCH 2 - lägg besöktea städer i array

int swa_helper(city* current, city* dest, int current_time, bool visited[]) {
    if (current == NULL) {
        return -1;
    }
    
    // If we've reached the destination, return the accumulated time.
    if (current == dest) {
        return current_time;
    }
    
    // Defensive: ensure the current city's id is within bounds.
    if (current->id < 0 || current->id >= 100) {
        fprintf(stderr, "Error: city id %d out of bounds\n", current->id);
        return -1;
    }
    
    // Mark this city as visited.
    visited[current->id] = true;

    // Explore each connection in a primitive manner:
    // We simply return the first valid path we find.
    for (connection* conn = current->connections; conn != NULL; conn = conn->next) {
        city* neighbor = conn->city;
        if (neighbor != NULL && !visited[neighbor->id]) {
            int result = swa_helper(neighbor, dest, current_time + conn->dist, visited);
            if (result != -1) {  // found a valid path, return immediately.
                return result;
            }
        }
    }

    // Backtrack: unmark the current city to allow for other paths.
    visited[current->id] = false;
    return -1;  // No valid path found from this branch.
}

// The main Search 2 function which returns the first valid path found.
int shortest_w_array(city* from, city* to) {
    // Allocate a visited array. (Assuming city ids are less than 100.)
    bool visited[100] = { false };
    
    return swa_helper(from, to, 0, visited);
}

// SEARCH 3 - dynamisk max limit

// Helper: DFS search that returns the minimal accumulated time from current to dest,
// or INT_MAX if no valid route under the time limit is found.
int dml_helper(city* current, city* dest, int current_time, int best_so_far, bool visited[]) {
    // If we've reached the destination, return the accumulated time.
    if (current == dest) {
        return current_time;
    }
    
    // If the current accumulated time already equals or exceeds the best found,
    // no need to continue exploring this branch.
    if (current_time >= best_so_far) {
        return best_so_far;
    }
    
    // Mark this city as visited.
    visited[current->id] = true;
    
    int best = best_so_far;
    // Explore each connection.
    for (connection* conn = current->connections; conn != NULL; conn = conn->next) {
        city* neighbor = conn->city;
        // Only explore unvisited neighbors.
        if (!visited[neighbor->id]) {
            int candidate = dml_helper(neighbor, dest, current_time + conn->dist, best, visited);
            if (candidate < best) {
                best = candidate;
            }
        }
    }
    
    // Backtrack: unmark the current city.
    visited[current->id] = false;
    return best;
}

// The main DFS function. Returns the shortest time found, or -1 if no path exists.
int shortest_w_dynamic_limit(city* from, city* to) {
    // Assuming city IDs are less than 100.
    bool visited[100] = { false };
    
    int best = dml_helper(from, to, 0, INT_MAX, visited);
    return (best == INT_MAX) ? -1 : best;
}

// Kolla kollisioner
void print_collision_stats(map* trains) {
    // Maximum possible collisions in a bucket (with 52 cities, max collisions is 51)
    int maxCollisions = 52;
    int collision_counts[maxCollisions];
    // Initialize the collision frequency array.
    for (int i = 0; i < maxCollisions; i++) {
        collision_counts[i] = 0;
    }
    
    // Iterate over each bucket in the hash table.
    for (int i = 0; i < MOD; i++) {
        int count = 0;
        // Each bucket pointer in the array might be NULL.
        if (trains->cities[i] != NULL) {
            city* current = trains->cities[i]->first;
            while (current != NULL) {
                count++;
                current = current->next;
            }
        }
        // Define collisions as number of cities minus one (if count is zero, then no collisions).
        int collisions = (count > 0) ? (count - 1) : 0;
        // Increment the frequency count for this number of collisions.
        collision_counts[collisions]++;
    }
    
    // Print out the collision statistics.
    for (int i = 0; i < maxCollisions; i++) {
        if (collision_counts[i] > 0) {
            if (i == 0) {
                printf("%d bucket(s) with no collisions.\n", collision_counts[i]);
            } else {
                printf("%d bucket(s) with %d collision(s).\n", collision_counts[i], i);
            }
        }
    }
}
// time for shorttest
void bench_shorttest(city* from, city* to, int limit) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Mätning 1:
    int s = shortest(from, to, limit);
 
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    if (s > 0) {
        printf("Shortest -> shortest path %d min found in %.2fms, limit was %d\n", s, ((double)wall)/1000000, limit);
    }
    else {
        printf("no path found\n");
    }
}

// time for shorttest w array
void bench_shorttest_w_array(city* from, city* to) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Mätning 2:
    int s = shortest_w_array(from, to);
    
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    if (s > 0) {
        printf("Shortest_w_array -> shortest path %d min found in %.2fms\n", s, ((double)wall)/1000000);
    }
    else {
        printf("no path found\n");
    }
}

//time for shorttest w array and dynamic limit
void bench_shorttest_w_dynamic_limit(city* from, city* to) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Mätning 3:
    int s = shortest_w_dynamic_limit(from, to);
    
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    if (s > 0) {
        printf("Shortest_w_dynamic -> shortest path %d min found in %.2fms\n", s, ((double)wall)/1000000);
    }
    else {
        printf("no path found\n");
    }
}
void Bench_MalmöToX(city* from, city* to) {
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    // Mätning 3:
    int s = shortest_w_dynamic_limit(from, to);
    
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    if (s > 0) {
        printf(",%d,%.2f,%.2f\n", s, ((double)wall)/1000000,s/(((double)wall)));
    }
    else {
        printf("no path found\n");
    }
}

/*
int main(int argc, char*argv[]) {

    map* trains = graph("trains.csv");

    // Kolla kollisioner:
    // print_collision_stats(trains);
    
    if(argc < 4) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        exit(1);
    }

    city* from = lookup(trains->cities, argv[1]);
    city* to = lookup(trains->cities, argv[2]);

    int limit = atoi(argv[3]);

    //bench_shorttest(from, to, limit);
    //bench_shorttest_w_array(from, to);
    bench_shorttest_w_dynamic_limit(from, to);

    return 0;
}
*/
int main(int argc, char*argv[]) {

    map* trains = graph("trains.csv");

    // Kolla kollisioner:
     //print_collision_stats(trains);
    
    if(argc < 2) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        exit(1);
    }

    
        char *citynames[] = {
            "Södertälje", "Norrköping", "Linköping", "Mjölby", "Nässjö",
            "Alvesta", "Hässleholm", "Lund", "Göteborg", "Varberg",
            "Halmstad", "Åstorp", "Skövde", "Herrljunga", "Falköping",
            "Värnamo", "Emmaboda", "Kristianstad", "Karlskrona", "Jönköping",
            "Katrineholm", "Hallsberg", "Örebro", "Arboga", "Stockholm",
            "Uppsala", "Gävle", "Sundsvall", "Ånge", "Umeå",
            "Boden", "Gällivare", "Borlänge", "Mora", "Sveg",
            "Östersund", "Avesta", "Storvik", "Fagersta", "Frövi",
            "Ludvika", "Västerås", "Eskilstuna", "Strömstad", "Uddevalla",
            "Trollhättan", "Helsingborg", "Kalmar", "Kiruna",
            "Luleå", "Sala"
        };

    city* from = lookup(trains->cities, argv[1]);
    for(int i = 0; i < 50; i++) {
        city* to = lookup(trains->cities, citynames[i]);
        printf("%s to %s",argv[1], citynames[i]);
        Bench_MalmöToX(from, to);
    }
    

    //bench_shorttest(from, to, limit);
    //bench_shorttest_w_array(from, to);

    return 0;
}