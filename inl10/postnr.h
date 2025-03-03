typedef struct area {
    char *name;
    char *zip;
    int pop;
} area;
typedef struct codes {
    area *areas;
    int n;
} codes;