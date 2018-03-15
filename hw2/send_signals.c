
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


// counter for number of signals sent

static int signals = 0;

void signal_handlers(int signum, siginfo_t* info, void* pv){

	// handling sigint and sigusr1
	switch(signum){
		// end after receiving sigint
		case SIGINT:
			printf("Signals: %d\n", signals);	
			exit(signum);
			break;
		// send sigusr2 back to whoever sent sigusr1
		case SIGUSR1:
			signals += 1;
			kill(info->si_pid, SIGUSR2);
			break;	
		default:
			return;
	}
}

// lets the child receive sigusr2
void usr2(int signum){
	if (signum == SIGUSR2)
		return;
}

void child_function(pid_t parent){
	
	struct sigaction s;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	s.sa_handler = usr2;
	//handle sigusr2
	sigaction(SIGUSR2, &s, NULL);

	// send 3 signals
	kill(parent,SIGUSR1);
	kill(parent,SIGUSR1);
	kill(parent,SIGUSR1);
	
	// send to end
	kill(parent,SIGINT);

	exit(0);
}


int main(){

	// parent process ID
	pid_t parent = getpid();

	// child process ID
	pid_t child;

	// sigaction struct
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_sigaction = signal_handlers;
	
	// handle receiving these two signals
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGUSR1, &sig, NULL);


	// create a child that will send sigusr1 to main
	if (child = fork() == child == 0){
		child_function(parent);
	}
	
	while(1)
		sleep(1);

}







