/*
create array of files
assign array of riders to files
create src file
populate the src file
distribute runs to 'read' files 
merge from  'read files' to 'write files'
swap riders from r to w
continue till one run remain
copy sorted seq. to out file 
*/
#include"ballanced_sort.h"

#define N 5 /*number of branches to split to */

int main(void) {
    char f_name[2*N][10]; /* automatic names only for branches max size 10*/
    struct Rider r_rider[N]; /*riders for read files  */
    struct Rider w_rider[N]; /*riders for write files */
    int i=0;
    int in_available = N;
    int in_active, run_active;

    /*generate file names and files */
    for (i = 0; i < 2*N; i++) {
        sprintf(f_name[i],"%d.txt",i); /*create file name */
        fprintf(stderr,"%d index:%s\n",i,f_name[i]);
        if (i < N) {
            set_rider(f_name[i],"w+",&r_rider[i]);
            close_rider(&r_rider[i]);
        }
        else {
            set_rider(f_name[i],"w+",&w_rider[i-N]);
            close_rider(&w_rider[i-N]);
        }
    }
    /* utilize the last file for initial sequence */
    /* don't need to create special file for it */
    set_rider(f_name[2*N-1],"w+",&w_rider[N]);
    fprintf(stderr,"Source seq. filename:%s\n",w_rider[N].filename);    
    generate_sequence(&w_rider[N],40,13);
    /***** distribute runs from generated seq to read files ****/

    /* assign riders for files to write to */
    for ( i = 0; i < N; i++) 
        set_rider(f_name[i],"w+",&w_rider[i]); 
    /* assign rider for source file */
    set_rider(f_name[2*N-1],"r",&r_rider[N-1]);
    i = 0;
    while (r_rider[N-1].curr != EOF) {
        copy_run(&r_rider[N-1],&w_rider[i]);
        i++;
        if (i == N) i = 0;
    }
    for ( i = 0 ; i < N; i++) 
        print_sequence(&w_rider[i]);
    for ( i = 0 ; i < N; i++) 
        close_rider(&w_rider[i]);
    
    
    return 0;
}

