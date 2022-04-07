#include <main.h>

/* returns a random decimal between 0 and 1 */
float getRandom() 
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

   float random = gsl_ran_flat(r,0,1);
   return random;
}


/* Function to change the traffic lights from red to green and vice versa */
void changeLights(bool *leftStop, bool *rightStop)
{
    /* Swap the booleans */
    *leftStop = !*leftStop;
    *rightStop = !*rightStop;
}

/* to add (or not) to the queue */
void add_to_queue(QUEUE *queue, int data, float prob) 
{
    /* add to queue or not */
    float leftRand = getRandom();
    if( leftRand < prob) {
        /* the counter is the iteration data */
        enqueue(queue,data);
    }
}

/* to remove (or not) from the queue */
void remove_from_queue(QUEUE *queue, float *dequeue_counter, float *wait_sum, int iter_counter, float *max_wait)
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


/* running one simulation */
void runOneSimulation(int rightTraffic, int leftTraffic, float add_to_left, float add_to_right, float arr[])
{
    /* Initialise the queues */
    QUEUE *leftQueue = malloc(sizeof(QUEUE));
    initialise(leftQueue);
    QUEUE *rightQueue = malloc(sizeof(QUEUE));
    initialise(rightQueue);

    /* Initialise the traffic lights */
    bool leftStop = false;
    bool rightStop = true;

    /* Initialise the iterations and dequeue counter */
    int iter_counter = 1;

    /* to break the main loop */
    bool isBroken = false;

    /* adding cut-off iteration */
    int cutoff = 500;

    /* right counters */
    float right_dequeue_counter = 0;
    float right_wait_sum = 0;
    float right_max_wait = 0;
    float right_empty_counter;
    float right_average_wait;

    /* left counters */
    float left_dequeue_counter = 0;
    float left_wait_sum = 0;
    float left_max_wait = 0;
    float left_empty_counter;
    float left_average_wait;

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
                    add_to_queue(leftQueue, iter_counter, add_to_left);

                    /* add to right queue or not */
                    add_to_queue(rightQueue, iter_counter, add_to_right);
                }

                /* remove left car */
                remove_from_queue(leftQueue, &left_dequeue_counter, &left_wait_sum, iter_counter, &left_max_wait);

                /* print info 
                printf("%d\n", iter_counter);
                printf("\nLeft Queue:\n\n");
                printf("Front: %d\n", leftQueue->front->car);
                printf("Back: %d\n", leftQueue->back->car);

                printf("\nRight Queue:\n\n");
                printf("Front: %d\n", rightQueue->front->car);
                printf("Back: %d\n", rightQueue->back->car);

                printf("~~~~~~~~~~~~~~~~~~~~~~~~\n"); */

                if(leftQueue->length == 0) {
                    left_empty_counter = iter_counter;
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

        /* if right is green */
        } else {
            /* loop rightTraffic amount of times */
            int i;
            for(i=0; i<rightTraffic; i++) {

                /* add cars */
                if( iter_counter < cutoff ) {
                    /* add to left queue or not */
                    add_to_queue(leftQueue, iter_counter, add_to_left);

                    /* add to right queue or not */
                    add_to_queue(rightQueue, iter_counter, add_to_right);
                }

                /* remove right car */
                remove_from_queue(rightQueue, &right_dequeue_counter, &right_wait_sum, iter_counter, &right_max_wait);
                

                /* print info 
                printf("%d\n", iter_counter);
                printf("\nLeft Queue:\n\n");
                printf("Front: %d\n", leftQueue->front->car);
                printf("Back: %d\n", leftQueue->back->car);

                printf("\nRight Queue:\n\n");
                printf("Front: %d\n", rightQueue->front->car);
                printf("Back: %d\n", rightQueue->back->car);

                printf("~~~~~~~~~~~~~~~~~~~~~~~~\n");*/
            


                if(rightQueue->length == 0) {
                    right_empty_counter = iter_counter;
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


void runSimulations(int leftTraffic, int rightTraffic, float add_to_left, float add_to_right)
{
    /* Initialise averaging variales */


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
        runOneSimulation(leftTraffic, rightTraffic, add_to_right, add_to_left, arr);

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


        /* print left info 
        printf("\nLeft Queue:\n\n");
        printf("Total vehicles passed through: %f\n", arr[0]);
        printf("Average waiting time: %f\n", arr[1]);
        printf("Maximum waiting time: %f\n", arr[2]);
        printf("Time taken to clear the queue (past 500): %f\n\n", arr[3]);

        printf("Right Queue:\n\n");
        printf("Total vehicles passed through: %f\n", arr[4]);
        printf("Average waiting time: %f\n", arr[5]);
        printf("Maximum waiting time: %f\n", arr[6]);
        printf("Time taken to clear the queue once (past 500): %f\n\n", arr[7]);
        

        printf("===================================================\n"); */
    }

    printf("\n\nParameter values:\n");
    printf("\tfrom left:\n");
    printf("\t\t traffic arrival rate: %f\n", add_to_left);
    printf("\t\t traffic light period: %d\n", leftTraffic);
    printf("\tfrom right:\n");
    printf("\t\t traffic arrival rate: %f\n", add_to_right);
    printf("\t\t traffic light period: %d\n", rightTraffic);

    printf("\nResults (averaged over 100 runs):\n");
    printf("\tfrom left:\n");
    printf("\t\t number of vehicles: %f\n", total_left_vehicles);
    printf("\t\t average waiting time: %f\n", total_left_wait_sum / (float)loop_num);
    printf("\t\t maximum waiting time: %f\n", total_left_max_wait);
    printf("\t\t clearance time: %f\n", total_left_clearance / (float)loop_num);

    printf("\tfrom right:\n");
    printf("\t\t number of vehicles: %f\n", total_right_vehicles);
    printf("\t\t average waiting time: %f\n", total_right_wait_sum / (float)loop_num);
    printf("\t\t maximum waiting time: %f\n", total_right_max_wait);
    printf("\t\t clearance time: %f\n", total_right_clearance / (float)loop_num);
}



int main ()
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

    /* Inputs */
    printf("Enter the left traffic rotation: \n");
    scanf("%d", &leftTraffic);
    printf("Enter the right traffic rotation: \n");
    scanf("%d", &rightTraffic);
    printf("Enter the probability to add to the left queue: \n");
    scanf("%f", &add_to_left); 
    printf("Enter the probability to add to the right queue: \n");
    scanf("%f", &add_to_right);
    

    runSimulations(leftTraffic, rightTraffic, add_to_left, add_to_right);
    /*runOneSimulation(rightTraffic, leftTraffic, add_to_right, add_to_left, arr, r); */

    
    return 0;
}