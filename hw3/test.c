
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char *name = argv[0];
	char *argm[] = {"ls", "-la", 0};
	int child_pid = fork();
	if (child_pid == 0) {
		execvp(argm[0],argm);
		printf("child of %s is %d\n",name,getpid());
	}else{
		printf("my child is %d\n", child_pid);
	}
}
