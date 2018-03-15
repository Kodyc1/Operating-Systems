
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char* argv[]) {

	pid_t child1;
	pid_t child2;
	pid_t child3;
	
	child1 = fork();
	
	if (child1 < 0){
	//	printf("fork failed\n");
	}
	
	else if (child1 == 0){	
		int i = 0;

		if (argc == 2){
			int x = atoi(argv[1]);
			for (i; i < x; i++){
				printf("B");
				fflush(stdout);
			}
		}
		else{
			int i = 0;
			for(i; i<10; i++){
				printf("B");
				fflush(stdout);
			}
		}				
		exit(0);				
	}

	else if (child1 > 0){
		child2 = fork();
		if (child2 == 0){
		


			int i = 0;

			if (argc == 2){
				int x = atoi(argv[1]);
				for (i; i < x; i++){
					printf("C");
					fflush(stdout);
				}
			}
			else{
				int i = 0;
				for(i; i<10; i++){
					printf("C");
					fflush(stdout);
				}
			}				
			exit(0);				
		}

		else{
			child3 = fork();

			if (child3 == 0){

				int i = 0;                              		
        
        			if (argc == 2){
        				int x = atoi(argv[1]);
        				for (i; i < x; i++){
        					printf("D");
        					fflush(stdout);
        				}
        			}
        			else{
        				int i = 0;
        				for(i; i<10; i++){
        					printf("D");
        					fflush(stdout);
        				}
				}
				exit(0);
		
			}

			else{
	       			int i = 0;
				if (argc == 2){
        				int x = atoi(argv[1]);
       					for (i; i < x; i++){
       						printf("A");
	       					fflush(stdout);
       					}
       				}
       				else{
       					int i = 0;
       					for(i; i<10; i++){
       						printf("A");
       						fflush(stdout);
       					}
       				}				
				exit(0);					
			}
		}
	}
	
	
	int n = 3;
	int status;
	pid_t pid;
	
	while(n>0){
		pid = wait(&status);
		--n;		
	}
	//printf("\n");
	return 0;

}
