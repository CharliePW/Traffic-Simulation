#ifndef __QUEUE__
#define __QUEUE__ 1

#include <stdio.h>
#include <stdlib.h>

/*----- structure definition -------------------------------------------------*/

struct queueNode
{
   int                  car;
   struct queueNode   *next;
};
typedef struct queueNode queueNode;

struct QUEUE 
{
    int length;
    queueNode *front;
    queueNode *back;
};
typedef struct QUEUE QUEUE;
/*----- function prototypes --------------------------------------------------*/

extern void   initialise    (QUEUE *);
extern void   enqueue       (QUEUE *, int);
extern int    dequeue       (QUEUE *);

#endif