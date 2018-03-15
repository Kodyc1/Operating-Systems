
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


int compare(const void* const A, const void* const B){
	return strcmp((*(struct dirent ** ) A)->d_name, (*(struct dirent **) B) -> d_name);
}


void recur (const char* dir_name){
	// directory
	DIR* directory;

	directory = opendir(dir_name); //open directory

	if (!directory){
		perror("can't open directory\n");
		return;
	}

	struct dirent* file;	// file struct

	struct dirent** list;        
	
	struct stat status;	// status struct

	char buf[256];		// path buffer


	int count = 0;
	int mem = 0;
	while((file = readdir(directory)) != NULL){
		if (strncmp(file->d_name,".", 1)!=0){
			count += 1;
			strcpy(buf,dir_name);
			strcat(buf,"/");
			strcat(buf,file->d_name);
			if (!stat(buf, &status))
				mem += status.st_blocks;
		}
	}
	
	
	list = malloc(count* sizeof(*list));
	if (list == NULL){
		fprintf(stderr, "memory\n");
		closedir(directory);
		return;	
	}
	else{
		rewinddir(directory);
	
		count = 0;
		while((file = readdir(directory)) != NULL){
			if (strncmp(file->d_name,".", 1)!=0){// && strcmp(file->d_name,"..") !=0 ){
				list[count++] = file;
			}
		}

		qsort(list, count, sizeof(*list), compare);

		int i = 0;
		printf("%s: \n", dir_name);
		printf("total %d\n", mem/2);
		
		// drwx------ 2 root root 6 date name
		for (i; i < count; i++){
			strcpy(buf, dir_name);
			strcat(buf, "/");
			strcat(buf, list[i]->d_name);
			if (!stat(buf, &status)){
				printf((S_ISDIR(status.st_mode)) ? "d" : "-");                   		
               			printf((status.st_mode & S_IRUSR) ? "r" : "-");
               			printf((status.st_mode & S_IWUSR) ? "w" : "-");
               			printf((status.st_mode & S_IXUSR) ? "x" : "-");
               			printf((status.st_mode & S_IRGRP) ? "r" : "-");	
               			printf((status.st_mode & S_IWGRP) ? "w" : "-");
               			printf((status.st_mode & S_IXGRP) ? "x" : "-");
               			printf((status.st_mode & S_IROTH) ? "r" : "-");
              			printf((status.st_mode & S_IWOTH) ? "w" : "-");
               			printf((status.st_mode & S_IXOTH) ? "x " : "- ");
                		
               			printf("%2d ",status.st_nlink);  //2
               			
               			struct passwd * u = getpwuid(status.st_uid); // root
               			struct group * g = getgrgid(status.st_gid);  // root
               			printf("%s ",u->pw_name);
               			printf("%s ",g->gr_name);
               		
               			printf("%5d ", status.st_size);	 // 6
               			char *modified;
               			modified =  ctime(&status.st_mtime); // date last modified

				char year[26];
				strcpy(year,ctime(&status.st_mtime));
				int y = atoi(&year[26] - 6);
				
				//printf("year: %d\n", y);		
              			if ( y == 2016){
					printf("%.*s ", 12, modified + 4);
				}
				else{
					printf("%.*s ", 6, modified + 4);
					printf("%.*s ", 5, modified + 19);
				}	
     				printf("%s ", list[i]->d_name);				
			}
			printf("\n");
		}
		printf("\n");
		closedir(directory);
	}

	directory = opendir(dir_name);

	while((file = readdir(directory)) != NULL){
		if (strcmp(file->d_name,".") != 0 && strcmp(file->d_name,"..") != 0){
			strcpy(buf, dir_name);
			strcat(buf, "/");
			strcat(buf, file->d_name);	
				
			if (!stat(buf,&status)){
				if (status.st_nlink > 1){
					//printf("%s\n", file->d_name);
					recur(buf);
				}
				else{
					// don't recur if not a directory;
				}
			}
		}
	}
	closedir(directory);   
	free(list);
}



int main(int argc, char *argv[]){

	struct stat status;
	// make sure there are two arguments: directory and ls
	if (argc != 2){
		perror("afk\n");
		exit(-1);
	}

	if(stat(argv[1], &status) == -1){
		perror("error\n");
		exit(-1);
	} 
	recur(argv[1]);


	return 0;
}
