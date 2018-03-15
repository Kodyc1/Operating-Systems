
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// create threads
pthread_t thread1, thread2, thread3;



// create mutex
pthread_mutex_t padlock;

// variables for calculating average
float total = 0;
int count = 0;

float min = 99999999;
float max = 0;

float input = 0;


void* thread_func(void* args){


	pthread_mutex_lock(&padlock);
	while(scanf("%f", &input) == 1)	
	{
		total += input;
		count += 1;

		if (input < min){
			min = input;
		}
		if (input > max){
			max = input;
		}
	
	}
	pthread_mutex_unlock(&padlock);

	pthread_exit(0);
}


int main(){

	// initialize mutex
	pthread_mutex_init(&padlock, NULL);

	// create threads before reading input
	pthread_create(&thread1, NULL, thread_func, NULL);
	pthread_create(&thread2, NULL, thread_func, NULL);
	pthread_create(&thread3, NULL, thread_func, NULL);



	if(pthread_join(thread1,NULL)){
		printf("thread1 error\n");
	}
	if(pthread_join(thread2,NULL)){
		printf("thread2 error\n");
	}
	if(pthread_join(thread3,NULL)){
		printf("thread3 error\n");
	}

	pthread_mutex_destroy(&padlock);
	
	
	float average = total/count;
	printf("max: %f\nmin: %f\naverage:%f\n", max, min, average);


	return 0;
}
