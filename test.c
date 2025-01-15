#include <stdio.h>
#include <stdlib.h>

int main() {
    
    int *b = (int*)malloc(sizeof(int)*10); 
    int c = 2;
    b[3] = 5;

    printf("hello");
    printf("hello");
    printf("hello");
    printf("hello");
    printf("hello");
    printf("hello");
    printf("hello");
    free(b);

    char a[] = "test";

    for (int i = 0; i < 8; i++)
    {
        printf("%s", a);
    }
    
}