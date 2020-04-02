#ifndef BALLANCED_SORT_H
#define BALLANCED_SORT_H 
#include<stdio.h>
#include<stdlib.h>

struct Rider {
    FILE *fp;
    const char *filename;
    int curr;
    int next;
    int eor;
    int eof;

};

void set_rider(const char *filename, const char *mode, struct Rider *rider_name);
void close_rider(struct Rider *rider_name);
void print_sequence(struct Rider *rider_name);
void copy_run(struct Rider *src, struct Rider *dest);
void generate_sequence(struct Rider *out, int size, int seed);
void read_next(struct Rider *src);

#endif
