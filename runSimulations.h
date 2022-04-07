#ifndef __MAIN__
#define __MAIN__ 1
 
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <stdio.h>
#include <queue.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/* ------ Function Prototypes -------- */
void changeLights (bool *, bool *);
void runOneSimaulation(int, int, float, float, float[], gsl_rng *);
void runSimulations(int, int, float, float, gsl_rng *);
void add_to_queue(QUEUE *, int , float, gsl_rng *);
void remove_from_queue(QUEUE *, float *, float *, int, float*, gsl_rng *);


#endif