#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#define HASH_SIZE 100

// Structure for a city connection
typedef struct connect {
    char *name;
    int time;
    struct connect *next;
} connect;

// Structure for a city
typedef struct city {
    char *name;
    int id;
    connect *connections;
} city;

// Hash table entry
typedef struct hash {
    char *name;
    city *pToCity;
} hash;

// Hash table
typedef struct table {
    hash citys[HASH_SIZE];
} table;

table cityTable;

// Hash function
unsigned int hashFunction(char *name) {
    unsigned int hash = 0;
    while (*name) {
        hash = (hash * 31) + *name++;
    }
    return hash % HASH_SIZE;
}

// Find or insert city in the hash table
city* findOrInsertCity(char *name) {
    unsigned int index = hashFunction(name);
    while (cityTable.citys[index].name != NULL) {
        if (strcmp(cityTable.citys[index].name, name) == 0) {
            return cityTable.citys[index].pToCity;
        }
        index = (index + 1) % HASH_SIZE;
    }
    
    // Insert new city
    cityTable.citys[index].name = strdup(name);
    cityTable.citys[index].pToCity = malloc(sizeof(city));
    cityTable.citys[index].pToCity->name = strdup(name);
    cityTable.citys[index].pToCity->connections = NULL;
    return cityTable.citys[index].pToCity;
}

// Add connection between cities
void addConnection(char *city1, char *city2, int time) {
    city *c1 = findOrInsertCity(city1);
    city *c2 = findOrInsertCity(city2);
    
    connect *newConn = malloc(sizeof(connect));
    newConn->name = strdup(city2);
    newConn->time = time;
    newConn->next = c1->connections;
    c1->connections = newConn;
}

// Read CSV file and populate hash table
void readCSV(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char city1[50], city2[50];
        int time;
        
        if (sscanf(line, "%49[^,],%49[^,],%d", city1, city2, &time) == 3) {
            addConnection(city1, city2, time);
        }
    }
    fclose(file);
}

// Find shortest path using a naive algorithm
int findShortestPath(char *start, char *end, int currentTime, int bestTime) {
    if (strcmp(start, end) == 0) {
        return currentTime;
    }
    
    city *currentCity = findOrInsertCity(start);
    connect *conn = currentCity->connections;
    
    while (conn) {
        int newTime = currentTime + conn->time;
        if (newTime < bestTime) {
            bestTime = findShortestPath(conn->name, end, newTime, bestTime);
        }
        conn = conn->next;
    }
    return bestTime;
}

void printGraph() {
    for (int i = 0; i < HASH_SIZE; i++) {
        if (cityTable.citys[i].name) {
            printf("City: %s\n", cityTable.citys[i].pToCity->name);
            connect *conn = cityTable.citys[i].pToCity->connections;
            while (conn) {
                printf("  -> %s (%d min)\n", conn->name, conn->time);
                conn = conn->next;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <start city> <end city>\n", argv[0]);
        return 1;
    }
    
    readCSV("trains.csv");
    
    int shortestTime = findShortestPath(argv[1], argv[2], 0, INT_MAX);
    if (shortestTime == INT_MAX) {
        printf("No path found from %s to %s\n", argv[1], argv[2]);
    } else {
        printf("Shortest travel time from %s to %s is %d minutes.\n", argv[1], argv[2], shortestTime);
    }
    
    return 0;
}
