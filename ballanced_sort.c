/*
- create array of files
-assign array of riders to files
-create src file
-populate the src file
-distribute runs to 'read' files 
-merge from  'read files' to 'write files'
-swap riders from r to w
-continue till one run remain
-copy sorted seq. to out file 
*/
#include"ballanced_sort.h"

#define N 5 /*number of branches to split to */
#define SEED 7  /* seed for pseudo random generator */
#define SIZE 34  /* no. of items in source sequence */

int main(void) {
    char f_name[2*N][10]; /* automatic names only for branches max name size 10*/
    struct Rider r_rider[N]; /*riders for read files  */
    struct Rider w_rider[N]; /*riders for write files */
    struct Rider tmp_rider; 
    int f_index[N]; /*file index for keeping track of active files to read from */
    int i = 0,run_count = 0, min_i = 0, w_i = 0, tmp;
    int in_available = N, min_val;
    int in_active, run_active;

/* *********Generate file names ********************** */
    fprintf(stderr,"Generated file names:\n");
    for (i = 0; i < 2*N; i++) {
        sprintf(f_name[i],"%d.txt",i); /*construct file name */
        fprintf(stderr,"%s\n",f_name[i]);
    }

/* ******* Generate initial sequence *************** */
    /* utilize the last file for initial sequence */
    /* don't need to create special file for it */
    set_rider(f_name[2*N-1],"w+",&w_rider[N-1]);
    generate_sequence(&w_rider[N-1],SIZE,SEED);

/* **** Distribute runs from generated seq to read files *** */

    /* assign riders for files to write to */
    for ( i = 0; i < N; i++) {
        set_rider(f_name[i],"w+",&w_rider[i]); 
    }
    /* assign rider for source file */
    set_rider(f_name[2*N-1],"r",&r_rider[N-1]);
    i = 0;
    /* distribute runs to output files */
    while (r_rider[N-1].curr != EOF) {
        copy_run(&r_rider[N-1],&w_rider[i]);
        i++, run_count++;
        if ( N == i) i = 0;
    }
    for ( i = 0 ; i < N; i++) {
        print_sequence(&w_rider[i]);
    }
    for ( i = 0 ; i < N; i++) {
        close_rider(&w_rider[i]);
    }
    close_rider(&r_rider[N-1]);

/*********** Merge runs from input files to output files ************/
    if (run_count < N) {
        in_active = run_count;
    }
    else in_active = in_available;
    
    run_count = 0;

    /* initial riders assignment to files and file index fill */
    for (i = 0; i < N; i++) {
        set_rider(f_name[i],"r+",&r_rider[i]);
        set_rider(f_name[i+N],"w+",&w_rider[i]);
        f_index[i] = i;
    }
    while (run_count != 1) {
        run_active = in_active;
        run_count = 0;
        w_i = 0;
        while ( in_active > 0) {
            while (run_active > 0) {
                
                i = 1, min_i = 0; 
                min_val = r_rider[f_index[min_i]].curr;

                /* search minimum value file index */
                while ( i < run_active ) {
                    if ( min_val > r_rider[f_index[i]].curr ) {
                        min_val = r_rider[f_index[i]].curr;
                        min_i = i;
                    }
                    i++;
                }
                
                fputc(min_val,w_rider[w_i].fp); /* write min_val to out.file */
                read_next(&r_rider[f_index[min_i]]); 

                /* manage active files index in case of eor and EOF*/
                if (r_rider[f_index[min_i]].eor == 1) {
                    r_rider[f_index[min_i]].eor = 0;
                    run_active--;
                    tmp = f_index[min_i], f_index[min_i] = f_index[run_active], f_index[run_active] = tmp;
                    if (r_rider[f_index[run_active]].eof == 1) {
                        fprintf(stderr, "EOF reached in %d\n",tmp);
                        r_rider[f_index[run_active]].eof = 0;
                        in_active--;
                        tmp = f_index[run_active], f_index[run_active] = f_index[in_active], f_index[in_active] = tmp;
                    }
                }
                
            } /* run_active > 0 */
            run_active = in_active;
            run_count++;
            w_i++;
            /* reset out file rider index */
            if (N == w_i) {
                w_i = 0;
            }


        } /* in_active > 0 */

        /* reset in active */
        if (run_count < N ) {
            in_active = run_count;
        }
        else in_active = N;

        /* swap riders, initial read curr and f_index reset */ 
        if (run_count != 1) {
            for (i = 0; i < in_active ; i++) {
                tmp_rider = r_rider[i];
                r_rider[i] = w_rider[i];
                rewind( r_rider[i].fp);
                w_rider[i] = tmp_rider;
                freopen(w_rider[i].filename, "w+", w_rider[i].fp);
                read_next(&r_rider[i]);
                f_index[i] = i;
            }
        }

    } /* run_count !=1 */

    print_sequence(&w_rider[0]);
    for (i = 0; i < N; i++) {
       // print_sequence(&r_rider[i]);
       // print_sequence(&w_rider[i]);
        close_rider(&r_rider[i]);
        close_rider(&w_rider[i]);   
    }

    
    return 0;
}

