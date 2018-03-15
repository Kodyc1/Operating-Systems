
#include <stdlib.h>
#include <stdio.h>

int main(){


// Input for number of processes P
// scanf("%d", integer);
// P1, P2, P3, P4, P5

	int processes;

	printf("Number of processes: ");
	scanf("%d", &processes);

  	
// Input for number of resources R
// scanf("%d", integer);
// A, B, C, D

	int resources;

	printf("Number of resources: ");
	scanf("%d", &resources);



// tracker
	int count;
   	int* running = malloc(sizeof(int)*processes);
	int z = 0;
	for (z; z < processes; z++){
		running[z] = 1;
		count += 1;
	}



// Max Resource Vector of R
// Total
// A B C D
// 8 5 9 7

	int* total = malloc(sizeof(int)*resources);
	printf("Enter %d available resources: ", resources);
	int i = 0;
	for (i; i<resources; i++){
		scanf("%d", &total[i]);
	}

// Current Resource Allocation Matrix
        	
	int*  current = malloc(sizeof(int)*(processes * resources));
	printf("Enter %d by %d numbers for current allocation: \n", processes, resources);
	int j = 0;
	for (j; j < processes * resources; j++){
		scanf("%d", &current[j]);
	}


// Available Resources 

	int* sum = malloc(sizeof(int)*resources);
	int x = 0;
	for (x; x < processes * resources; x ++){
		
		int y = x % resources;
	
		sum[y] += current[x];
	}
	
	int* available = malloc(sizeof(int)* resources);
	for(x = 0; x < resources; x ++ ){
		available[x] = total[x] - sum[x];
		
	}
	/*
	for (x = 0; x < resources; x ++ ){
		printf("\t%d", available[x]);
	}
	*/


// Maximum Resource Allocation Matrix

	int* maximum = malloc(sizeof(int)*(processes * resources));
	printf("Enter %d by %d numbers for maximum allocations: \n", processes, resources);
	int k = 0;
	for (k; k < processes * resources; k++){
		scanf("%d", &maximum[k]);
	}

// Need Matrix

	int *need = malloc(sizeof(int)* (processes * resources));
	int l = 0;
	for (l; l < processes * resources; l++){
		need[l] = maximum[l] - current[l];
	}




// Banker's Algorithm 
	int runs = 0;
	while (count != 0){

		int unsafe = 1;

		for (i = 0; i < processes; i++){
			if (running[i]){
				runs = 1;
				for (j = 0; j < resources; j++){
					if (need[i * resources + j]  > available[j]){
						runs = 0;
						break;
					}
				}
				if (runs){
					running[i] = 0;
					count -= 1;
					unsafe = 0;
				
					for (j = 0; j < resources; j++){
						available[j] += current[i * resources + j];
					}

					break;
				}
			}
		}	


		if (unsafe){
			printf("\nThe system is in an unsafe state.\n");
			break;
		}
		else{
			//printf("\nSafe");
			printf("\nAvailable vector");
		
			for ( i = 0; i < resources; i++){
				printf("\t%d", available[i]);
			}
			printf("\nThe system is in a safe state.\n");
		}

	}	

	//printf("The system is in a safe state.\n");
// Output Safe/NOT Safe

	//printf("\nThe system is in a safe state\n");


	//printf("\nThe system is in an unsafe state\n");

	free(running);
	free(total);
	free(available);
	free(sum);
	free(need);
	free(current);
	free(maximum);

	return 0;
}
