#ifndef __MAIN__
#define __MAIN__ 1

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <stdio.h>
#include <queue.h>
#include <stdbool.h>


/* ------ Function Prototypes -------- */
void changeLights (bool *, bool *);
void runOneSimaulation(int, int, float, float, float[]);
void runSimulations(int, int, float, float);
void add_to_queue(QUEUE *, int , float);
void remove_from_queue(QUEUE *, float *, float *, int, float*);
float getRandom();


#endif