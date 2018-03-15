
#include <stdlib.h>
#include <stdio.h>  // for printf and NULL
#include <unistd.h> // for fork 
#include <string.h> 

int main(){

	char* line = NULL;
	size_t length = 0;;
	ssize_t read;

	char* argv[100];
	//char* env;
	//env = getenv("PATH");
	pid_t child;
	int status;
	

	while(1){
		// shell 
		printf("$ ");
		fflush(stdout);
		
		// ssize_t getline(char** lineptr, size_t *n, FILE *stream)
		read = getline(&line, &length, stdin);
		//printf("%s", line);
		

		// getline returns -1 at EOF
		if(read < 0) {
			break; }
		
		// remove newline from stdin
		if (line[read-1] == '\n') {
			line[read-1] = '\0'; }
		

		// take the commands and split them by spaces
		char* token;

		token = strtok(line," ");
		
		if (token == NULL){	}
		else{
			int i = 0;
			while (token != NULL){
			//	printf("%s", token);
				argv[i] = token;
				token = strtok(NULL, " ");
				i++;
			}
			argv[i] = NULL;

			// fork a child to exec

			child = fork();
		
			if (child == 0){
				// int execvpe(const char *file, char *const argv[], char *const envp[]);
				execvp(argv[0],argv); //env
				printf("Invalid command\n");
				exit(0);
			}
			else {  //parent waits for child
				wait(NULL);			
				//printf("reaped child\n");
			}
		}
	}
	child = wait(&status);
	free(line);
	return 0;
}


