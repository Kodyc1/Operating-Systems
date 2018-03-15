
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>


struct pair{
	struct dirent * file;
	int block;
};


int compare(const void* a, const void* b){	

 	struct pair A = *(struct pair*) a;
	struct pair B = *(struct pair*) b;
		
	//printf("%d\n", A.block);
	//printf("%d\n", B.block);

	return (A.block - B.block);
}


void recur( const char* dir_name){

	DIR * directory;

	struct dirent * file;

	struct stat status;	


//	struct statvfs statusvfs;

	char buf[256];
	

	directory = opendir(dir_name);
	if(!directory){
		perror("can't open directory\n");
		return;
	}



	int count = 0;
	
	int mem = 0;
	while((file = readdir(directory)) != NULL){

		if(strcmp(file->d_name, ".")!= 0 && strcmp(file->d_name,"..") != 0){
			strcpy(buf, dir_name);
			strcat(buf, "/");
			strcat(buf, file->d_name);
		 	if (!stat(buf, & status)){
				int block = (status.st_blocks)/2;
				mem += block;
				count++;
			}	
			
		}
	}

	struct pair* list = malloc(count* sizeof(struct pair));

	if(list == NULL){
		fprintf(stderr, "memory\n");
		closedir(directory);
		return;
	}

	else{
		rewinddir(directory);
		count = 0;

		while ((file = readdir(directory))!=NULL){
			if(strncmp(file->d_name,".", 1)!=0){
				strcpy(buf,dir_name);
				strcat(buf,"/");
				strcat(buf, file->d_name);

				if (!stat(buf,&status)){
					//list[count] = (struct pair) {file, status.st_blocks/2};
					if (status.st_nlink > 1){
						list[count].file = file;
						list[count].block = (status.st_blocks + 8)/2;
						count++;
					}
				}
			}
		} 

		
		qsort(list,count,sizeof(struct pair),compare);
		
		int i = 0;
		printf("%d\t%s\n", mem+4, dir_name);
		for(i; i< count; i++){

//			printf("%d\t%s\n", list[i].block, list[i].file->d_name);

			strcpy(buf,dir_name);
			strcat(buf,"/");
			strcat(buf, list[i].file->d_name);
			if (!stat(buf, &status)){
				if (status.st_nlink > 1){
//					printf("%d\t", status.st_blocks);
//					printf("%s/%s\n", dir_name,list[i].file->d_name);
				}
			}
//			if (i == count-1){
//				printf("%d\t%s\n", mem,dir_name);	} 
			
		}
	

//		printf("%d\t%s\n", mem+4 ,dir_name);


		closedir(directory);
	}




	directory = opendir(dir_name);
	while ((file = readdir(directory)) != NULL){
		if (strcmp(file->d_name,".") != 0 && strcmp(file->d_name,"..") != 0){
			strcpy(buf, dir_name);
			strcat(buf, "/");
			strcat(buf, file->d_name);		

			if (!stat(buf, &status)){
				if (status.st_nlink > 1){
					recur(buf);
				}
				else{}
			}
		}
	}
	closedir(directory);

	free(list);

}




int main(int argc, char * argv[]){

	struct stat status;

        recur(argv[1]);	

	// if it's directory
	// 	printf ( block size );  
	// 	printf ( buf / file_name);


	return 0;
}
