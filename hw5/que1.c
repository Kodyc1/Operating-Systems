#include <stdio.h>
#include <stdlib.h>
#include "que.h"
#include <pthread.h>
#include <semaphore.h>

//QUE MAX = 10
static ELE _que[QUE_MAX];
static int _front = 0, _rear = 0;
extern int producers_working;

static int matches = 0;

// create a mutex
pthread_mutex_t crit;

// create semaphores
sem_t full, empty, mutex, m;
pthread_cond_t sig;


void add_match()
{
    //Note: you will need to lock this update because it is a race condition
//    pthread_mutex_lock(&m);
    sem_wait(&m);
    matches++;
    sem_post(&m);
  //  pthread_mutex_unlock(&m);
}

void report_matches(char *pattern)
{
    // destroy mutex and semaphores
    //pthread_mutex_destroy(&m);
    //pthread_mutex_destroy(&crit);
    //sem_destroy(&full);
    //sem_destroy(&empty);
    //sem_destroy(&mutex);		

    if (producers_working == 0){
	 que_error("hello here");
     //    pthread_cond_signal(&sig);
    }

    printf("Found %d total matches of '%s'\n", matches, pattern);
    // destroy mutex and semaphores
    sem_destroy(&m);
    pthread_mutex_destroy(&crit);
    pthread_cond_destroy(&sig);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);		
}

int que_init()
{   // initialize mutex
    sem_init(&m, 0,1);
    pthread_mutex_init(&crit, NULL);
    pthread_cond_init(&sig, NULL);
    // initialize enqueue wait when buffer is full
    sem_init(&full, 0, 0);
 
    // initialize dequeue wait when buffer is empty
    sem_init(&empty, 0, QUE_MAX);

    sem_init(&mutex, 0, 1);    
}

void que_error(char *msg)
{
    fprintf(stderr, "***** Error: %s\n", msg);
    //exit(-1);
}

int que_is_full()
{
    return (_rear + 1) % QUE_MAX == _front; /* this is why one slot is unused */
}


int que_is_empty()
{
    if (producers_working == 0){
        que_error("HELLO THERE");
        pthread_cond_signal(&sig);
    }
    return _front == _rear;
}


void que_enq(ELE v)
{
    // replace this spin with something better.....
//    while (que_is_full()) // note this is not right
    
    sem_wait(&empty);
          
    pthread_mutex_lock(&crit);
 //  sem_wait(&mutex);


    if ( que_is_full() )
        que_error("enq on full queue");


    _que[_rear++] = v;
   
    if ( _rear >= QUE_MAX )
        _rear = 0; 
    pthread_cond_signal(&
 //   sem_post(&mutex);    
    pthread_mutex_unlock(&crit);
    
    sem_post(&full);
}


ELE que_deq()
{
    // replace this spin with something better.....
    //while (producers_working && que_is_empty()) // note this is not right
      //  sem_wait(&full);


    ELE none;    
    if (producers_working == 0){
        que_error("producers stopped working");
       
	return none;
    }

    sem_wait(&full);

    pthread_mutex_lock(&crit);
 //   sem_wait(&mutex); 
    
    pthread_cond_wait(&sig, &crit);
    if (producers_working == 0){
        pthread_cond_signal(&sig);
        que_error("producers are no longer functioning");
    
    }
    if ( que_is_empty() )
        que_error("deq on empty queue");
   // pthread_cond_wait(&sig, &crit);
    ELE ret = _que[_front++];
    if ( _front >= QUE_MAX )
        _front = 0;

   //sem_post(&mutex);    
    pthread_mutex_unlock(&crit);

    sem_post(&empty);
    
    return ret;
   
}



/*

int main()
{
    for ( int i=0; i<QUE_MAX-1; ++i )
    {
        Buffer b;
        sprintf(&b.string, "%d", i);
        que_enq(b);
    }
    while ( !que_is_empty() )
        printf("%s ", que_deq().string);
    putchar('\n');
}
*/
