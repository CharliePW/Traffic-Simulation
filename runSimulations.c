#include <runSimulations.h>


/* Function to change the traffic lights from red to green and vice versa */
void changeLights(bool *leftStop, bool *rightStop)
{
   /* Swap the booleans */
   *leftStop = !*leftStop;
   *rightStop = !*rightStop;
}


/* to add (or not) to the queue */
void add_to_queue(QUEUE *queue, int data, float prob, gsl_rng *r) 
{
   /* add to queue or not */
   float leftRand = gsl_ran_flat(r,0,1);
   if( leftRand < prob) {
      /* the counter is the iteration data */
      enqueue(queue,data);
   }
}


/* to remove (or not) from the queue */
void remove_from_queue(QUEUE *queue, float *dequeue_counter, float *wait_sum, int iter_counter, float *max_wait, gsl_rng *r)
{
   /* remove car */
   if(queue->length > 0) {
      /* check values */
      *dequeue_counter += 1;
      int current_wait = iter_counter - queue->front->car;
      *wait_sum += current_wait;
      if( current_wait > *max_wait ) {
         *max_wait = current_wait;
      } 
      /* dequeue queue */
      dequeue(queue);
   }
}

/*running one simulation */
void runOneSimulation(int rightTraffic, int leftTraffic, float add_to_left, float add_to_right, float arr[], gsl_rng *r)
{
   /* Initialise the queues */
   QUEUE *leftQueue = malloc(sizeof(QUEUE));
   if(leftQueue == NULL) {
      fprintf(stderr,"left queue memory error!\n");
      exit(-1);
   }
   initialise(leftQueue);

   QUEUE *rightQueue = malloc(sizeof(QUEUE));
   if(leftQueue == NULL) {
      fprintf(stderr,"right queue memory error!");
      exit(-1);
   }
   initialise(rightQueue);
 
   /* Initialise the traffic lights */
   bool leftStop = false;
   bool rightStop = true;
  
   /* Initialise the iterations and dequeue counter */
   int iter_counter = 1;
  
   /* to break the main loop */
   bool isBroken = false;
 
   /* adding cutoff iteration */
   int cutoff = 500;

   /* right counters */
   float right_dequeue_counter = 0;
   float right_wait_sum = 0;
   float right_max_wait = 0;
   float right_empty_counter;
   float left_average_wait;

   /* left counters */
   float left_dequeue_counter = 0;
   float left_wait_sum = 0;
   float left_max_wait = 0;
   float left_empty_counter;
   float right_average_wait;  

   /* One car waiting in each queue at the beginning */
   enqueue(leftQueue,0);
   enqueue(rightQueue,0);

   /* main loop */
   while(true) {

      /* if left is green */
      if(!leftStop) {
         
	 /* loop leftTraffic amount of times */
         int i;
         for(i=0; i<leftTraffic; i++) {
            /* add cars */
            if( iter_counter < cutoff ) {
               /* add to left queue or not */
               add_to_queue(leftQueue, iter_counter, add_to_left, r);

 	       /* add to right queue or not */
	       add_to_queue(rightQueue, iter_counter, add_to_right, r);
            }
	         
            /* remove left car */
            remove_from_queue(leftQueue, &left_dequeue_counter, &left_wait_sum, iter_counter, &left_max_wait, r);
            
            if(leftQueue->length == 0) {
               left_empty_counter = iter_counter - cutoff;
            }
	     
            if(iter_counter > cutoff && (leftQueue->length == 0 && rightQueue->length == 0)) {
               isBroken = true;
               break;
            }
  	    iter_counter++;
	 }  

         if(isBroken) {
            break;
         }
               
         changeLights(&leftStop,&rightStop);
         iter_counter++;
         

      /*if right is green */
      } 
      if(!rightStop) {
         
         /* loop leftTraffic amount of times */
         int i;
         for(i=0; i<rightTraffic; i++) {
            /* add cars */
            if( iter_counter < cutoff ) {
               /* add to left queue or not */
               add_to_queue(leftQueue, iter_counter, add_to_left, r);
              
               /* add to right queue or not */
               add_to_queue(rightQueue, iter_counter, add_to_right, r);
            }
            /* remove right car */
            remove_from_queue(rightQueue, &right_dequeue_counter, &right_wait_sum, iter_counter, &right_max_wait, r);
              
            if(rightQueue->length == 0) {
               right_empty_counter = iter_counter-cutoff;
            }
            
            if(iter_counter > cutoff && (rightQueue->length == 0 && rightQueue->length == 0)) {
               isBroken = true;
               break;
            }
	         iter_counter++;
         }       
      
	      if(isBroken) {
            break;
         }

         changeLights(&leftStop,&rightStop);
         iter_counter++;
      }   
   }

   arr[0] = left_dequeue_counter;
   left_average_wait = left_wait_sum / left_dequeue_counter;
   arr[1] = left_average_wait;
   arr[2] = left_max_wait;
   arr[3] = left_empty_counter;
   arr[4] = right_dequeue_counter;
   right_average_wait = right_wait_sum / right_dequeue_counter;
   arr[5] = right_average_wait;
   arr[6] = right_max_wait;
   arr[7] = right_empty_counter;
   
   free(leftQueue);
   free(rightQueue);
}


/* Running the multiple simulations */
void runSimulations(int leftTraffic, int rightTraffic, float add_to_left, float add_to_right, gsl_rng *r)
{
   /* left counters */
   float total_left_vehicles = 0;
   float total_left_wait_sum = 0;
   float total_left_max_wait = 0;
   float total_left_clearance;

   /* right counters */
   float total_right_vehicles = 0;
   float total_right_wait_sum = 0;
   float total_right_max_wait = 0;
   float total_right_clearance;

   /* return array */
   float arr[8];
  
   int loop_num = 100;

   
   /* loop loop_num times */
   int i;
   for(i=0; i<loop_num; i++) {
      /* one simulation values */
      runOneSimulation(leftTraffic, rightTraffic, add_to_right, add_to_left, arr, r);
      
      total_left_vehicles += arr[0];
      total_left_wait_sum += arr[1];
      if(arr[2] > total_left_max_wait) {
         total_left_max_wait = arr[2];
      }
      total_left_clearance += arr[3];

      total_right_vehicles += arr[4];
      total_right_wait_sum += arr[5];
      if(arr[6] > total_right_max_wait) {
         total_right_max_wait = arr[2];
      }
      total_right_clearance += arr[7];
   }

   printf("\n\nParameter values:\n");
   printf("\tfrom left:\n");
   printf("\t\t traffic arrival rate: %.2f\n", add_to_left);
   printf("\t\t traffic light period:   %d\n", leftTraffic);
   printf("\tfrom right:\n");
   printf("\t\t traffic arrival rate: %.2f\n", add_to_right);
   printf("\t\t traffic light period:   %d\n", rightTraffic);

   printf("\nResults (averaged over 100 runs):\n");
   printf("\tfrom left:\n");
   printf("\t\t number of vehicles:   %.2f\n", total_left_vehicles / (float)loop_num);
   printf("\t\t average waiting time:  %.2f\n", total_left_wait_sum / (float)loop_num);
   printf("\t\t maximum waiting time:  %.2f\n", total_left_max_wait);
   printf("\t\t clearance time:       %.2f\n", total_left_clearance / (float)loop_num);

   printf("\tfrom right:\n");
   printf("\t\t number of vehicles:    %.2f\n", total_right_vehicles / (float)loop_num);
   printf("\t\t average waiting time:  %.2f\n", total_right_wait_sum / (float)loop_num);
   printf("\t\t maximum waiting time:  %.2f\n", total_right_max_wait);
   printf("\t\t clearance time:        %.2f\n", total_right_clearance / (float)loop_num);
   printf("\n\n");
}


/* Main function */
int main(int argc, char **argv) 
{
   /* Random Number Stuff */
   const gsl_rng_type *T;
   gsl_rng *r;
   /* create a random number generator */
   gsl_rng_env_setup();
   T = gsl_rng_default;
   r = gsl_rng_alloc(T);
   /* seed it – equivalent of srand(time(0)) */
   gsl_rng_set(r,time(0));

   /* Initialise the parametres */
   float add_to_left, add_to_right;
   int leftTraffic, rightTraffic;

   /* return array */
   float arr[8];

   
   if(argc != 5) {
      fprintf(stderr, "You must enter 4 intergers!\n");
      exit(-1);
   }

   int i;
   int j;
   for(i=1; i<argc; i++) {
      for(j=0; j<(int)strlen(argv[i]); j++) {
         if(!isdigit(argv[i][j])) {
            fprintf(stderr, "You must enter an integers!\n");
            exit(-1);
         }
      }
   }

   if(atoi(argv[1]) < 1 || atoi(argv[2]) < 1) {
      fprintf(stderr,"You cannot enter 0!\n");
      exit(-1);
   }
   
   
   leftTraffic = atoi(argv[1]);
   rightTraffic = atoi(argv[2]);
   add_to_left = (float)atoi(argv[3]) / (float)100;
   add_to_right = (float)atoi(argv[4]) / (float)100;

   runSimulations(leftTraffic, rightTraffic, add_to_left, add_to_right, r);
  
   return 0;  
}
