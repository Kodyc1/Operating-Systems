
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>


// count for number of times buttons pressed
static int interrupt = 0;
static int quit = 0;
static int count = 0;

// handler function
void init_signal_handlers(int signum){

	//SIGINT  (CTRL + C)
	//SIGQUIT (CTRL + Z)
	//SIGTSTP (CTRL + \)
	
	//print the letter for each signal, increase count, and exit after 3 stops
	switch(signum){
		case SIGINT:
			printf("I");
			fflush(stdout);
			interrupt += 1;
			break;
	
		case SIGQUIT:
			printf("Q");
			fflush(stdout);
			quit += 1;
			break;
		case SIGTSTP:
			printf("S");
			fflush(stdout);
			count += 1;
			if (count == 3){
				printf("\nInterrupt: %d\nStop: %d\nQuit: %d\n", interrupt, quit, count);
				exit(signum);
			}
			break;
		default:
			return;
	}
}			

int main(){
	
	// sigaction struct
	struct sigaction Handler;

	// sigaction assigning handler
	Handler.sa_handler = init_signal_handlers;
	sigemptyset(&Handler.sa_mask);
	Handler.sa_flags = 0;
	
	sigaction(SIGINT, &Handler, NULL);
	
	sigaction(SIGQUIT, &Handler, NULL);

	sigaction(SIGTSTP, &Handler, NULL);

	// signal(SIGINT, init_signal_handlers);
	// signal(SIGQUIT, init_signal_handlers);
	while(1)
		sleep(1);
	
}








