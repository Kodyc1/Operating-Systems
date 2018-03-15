
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF 1000

pthread_t thread1, thread2, thread3;
//int first, second, third;

float arr[BUF];

float total = 0;
int count = 0;

float min = 99999999;
float max = 0;



// struct for passing arguments in
typedef struct argstruct {
	int start;
	int finish;
}argstruct;



// thread functions come in here
void* thread_func (void* arg_ptr)
{
	argstruct* args = (argstruct*)  arg_ptr;	

	int i = args->start;
	for(i; i < args->finish; i++){
	
		total += arr[i];
		
		if (arr[i] > max){
			max = arr[i];
		}
		if (arr[i] < min){
			min = arr[i];
		}
	}

	pthread_exit(0);
	return NULL;
}


// thread version of compute
int main(){

	float input = 0;

	// place all the numbers into buffer	
	while(scanf("%f",&input) == 1)
	{
		arr[count] = input;
		count += 1;	
	}				


	// divide numbers into 4 sections
	int extra = count % 4;
	int quarter = (count-extra)/4;
	

	int j = 0;
	for (j; j < quarter; j++){
		total += arr[j];
		if (arr[j] > max){
			max = arr[j];
		}
		if (arr[j] < min){
			min = arr[j];
		}
	}


	// pass arguments as structs
	struct argstruct struct1;
	struct1.start = quarter;
	struct1.finish = quarter * 2;

	struct argstruct struct2;
	struct2.start = quarter * 2;
	struct2.finish = quarter * 3;

	struct argstruct struct3;
	struct3.start = quarter * 3;
	struct3.finish = quarter * 4 + extra;


	// create three threads
	// int = pthread_create(thread pointer, attr or default NULL,
	// 			pointer to function for thread, struct of args)

	//first
	pthread_create(&thread1, NULL, thread_func, (void*) &struct1);
	//second
	pthread_create(&thread2, NULL, thread_func, (void*) &struct2);
	//third
	pthread_create(&thread3, NULL, thread_func, (void*) &struct3);


	if(pthread_join(thread1, NULL)){
		printf("error joining 1\n");
	}
	if(pthread_join(thread2, NULL)){
		printf("error joining 2\n");
	}
	if(pthread_join(thread3, NULL)){
		printf("error joining 3\n");
	}

//	printf("\nfinished pid: %d\n", getpid());
//	printf("first thread returned: %d\n", first);
//	printf("second thread returned: %d\n", second);
//	printf("third thread returned: %d\n", third);


	float average = total/count;
	printf("max: %f\nmin: %f\naverage: %f\n", max, min, average);

	return 0;
}






