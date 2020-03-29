#include"ballanced_sort.h"

void set_rider(const char *filename, const char *mode, struct Rider *rider_name) {
    if ((rider_name->fp = fopen(filename,mode)) == NULL)
        fprintf(stderr,"File open for \"%s\" failed!\n",filename), exit(1);
    rider_name->filename = filename;
    if (mode[0] != 'w') /*no read first item if file open for write */ 
        rider_name->curr = fgetc(rider_name->fp);
    rider_name->next = EOF;
    rider_name->eor = 0;
    rider_name->eof = 0;
}
void close_rider(struct Rider *rider_name) {
    if (rider_name->fp != NULL)
        if((fclose(rider_name->fp)) != 0)
            fprintf(stderr,"Closing of \"%s\" failed!\n",rider_name->filename);
            

}

void print_sequence(struct Rider *rider_name) {
    rewind(rider_name->fp);
    fprintf(stderr,"%s:\n",rider_name->filename);
    while((rider_name->curr = fgetc(rider_name->fp))!= EOF) 
        fprintf(stderr,"%d,", rider_name->curr);
    fprintf(stderr,"\n");
}

void copy_run(struct Rider *src, struct Rider *dest) {
    
 while((src->curr !=EOF) && (src->eor != 1)) {   
     fputc(src->curr,dest->fp);
     if ((src->next = fgetc(src->fp)) < src->curr) 
         src->eor = 1;
     src->curr = src->next;

    }
src->eor = 0;
}

void generate_sequence(struct Rider *out, int size,int seed) {
/* ********* create initial pseudo-random sequence and store in file ***** */
    int i = 0;
    fprintf(stderr,"Generated sequence in %s:\n", out->filename);
    while ( i < size) {
        
        if (seed < 256) {
            fprintf(stderr,"%d,", seed);
            fputc(seed, out->fp);
            i++;
        }
        seed = (31*seed)% 997 + 5;
    }
    fprintf(stderr,"\n");
    close_rider(out); 
}

void read_next(struct Rider *src) {
    src->next = fgetc(src->fp);
    if (src->next < src->curr)
        src->eor = 1;
    if (src->next == EOF)
        src->eof = 1;
    src->curr = src->next;
}
