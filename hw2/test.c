


#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



void sighandler(int signum){
//	signal(SIGINT,sighandler);
	printf("\nmoshimoshi\n");
	fflush(stdout);
	exit(1);
}

int main(){

	signal(SIGINT,sighandler);
	
	while(1){
		sleep(1);
	}
}
