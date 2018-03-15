#include <stdio.h>
#include <stdlib.h>
#include "que.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
static ELE _que[QUE_MAX];
static int _front = 0, _rear = 0;
extern int producers_working;

static int matches = 0;

pthread_mutex_t m;

pthread_mutex_t locks;

pthread_cond_t full;

extern pthread_cond_t empty;


struct timespec timeout;

time_t T;


void add_match()
{
    //Note: you will need to lock this update because it is a race condition
    pthread_mutex_lock(&m);
    matches++;
    pthread_mutex_unlock(&m);
}

void report_matches(char *pattern)
{
    pthread_mutex_destroy(&m);
    pthread_mutex_destroy(&locks);
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);


    printf("Found %d total matches of '%s'\n", matches, pattern);
}

int que_init()
{
    pthread_mutex_init(&m, NULL);
    pthread_mutex_init(&locks, NULL);

    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

}

void que_error(char *msg)
{
    fprintf(stderr, "***** Error: %s\n", msg);
    // exit(-1);
}

int que_is_full()
{
    return (_rear + 1) % QUE_MAX == _front; /* this is why one slot is unused */
}

int que_is_empty()
{
    //if (producers_working == 0){
    //     pthread_cond_broadcast(&empty);
    //}
    return _front == _rear;
}

void que_enq(ELE v)
{

    pthread_mutex_lock(&locks);
 
    // replace this spin with something better.....
    while (que_is_full()) 
         pthread_cond_wait(&full,&locks); 
         

    if ( que_is_full() ){
         que_error("enq on full queue");
    }

    _que[_rear++] = v;
    if ( _rear >= QUE_MAX )
        _rear = 0;


    pthread_cond_signal(&empty);


    pthread_mutex_unlock(&locks);


}

ELE que_deq()
{
    // lock 
    pthread_mutex_lock(&locks);
 
    time(&T);
    timeout.tv_sec = T + 2;
    int err = 1;

    // replace this spin with something better.....
    while (producers_working && que_is_empty()){ 
         err = pthread_cond_timedwait(&empty, &locks, &timeout);
         if (err == ETIMEDOUT){       
	     break;               
         }
    }
     
    if (que_is_empty()){
       
        que_error("deq on empty queue");

    }   
 
    ELE ret = _que[_front++];
    if ( _front >= QUE_MAX )
        _front = 0;
            
    pthread_cond_signal(&full);

    pthread_mutex_unlock(&locks);
   
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
