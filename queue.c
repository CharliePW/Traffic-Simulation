#include <queue.h>

/*------ local function prototypes ----*/

static queueNode *newNode (int car);

/*------ Queue Functions -----*/

/* Initialise a queue */
extern void initialise(QUEUE *queue)
{
   queue->length = 0;
   queue->front  = NULL;
   queue->back   = NULL;
}

/* create a new node from the given data and return a pointer to it*/
static queueNode *newNode(int car)
{
   queueNode *q = (queueNode *)malloc(sizeof(queueNode));
   if (q==NULL) {
      fprintf(stderr,"Memory allocation error\n");
      exit(-1);
   }
   q->car = car;
   q->next = NULL;
   return q;
}


/* add a car to the end of the queue */
extern void enqueue(QUEUE *queue, int car)
{
   /*create a new node */
   queueNode *q = newNode(car);

   /* if queue is empty, then node is front and back */
   if(queue->back == NULL) {
      queue->front = queue->back = q;
   } else {
      /* add node to the end of the queue */
      queue->back->next = q;
      queue->back = q;
      queue->length++;
   }
}


/* release a car from the front of the queue */
extern int dequeue(QUEUE *queue)
{
   queueNode *q = queue->front;
   int car = q->car;

   if(q==NULL) {
      fprintf(stderr,"Warning: attempting to dequeue from an empty queue.\n");                
      fprintf(stderr,"Warning: line %d, file %s\n",__LINE__,__FILE__);
      return car;
   }
   
   queue->front = queue->front->next;
   queue->length--;
   free(q);
   return car;
}

