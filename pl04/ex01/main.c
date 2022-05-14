#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>

int NUMBER_OF_CHILDREN = 8;

int main(){
	
	remove("Output.txt");
	
	/**
	 * Semaphore creation
	 * */
	 sem_t *sem_read,*sem_write;
	 if((sem_read = sem_open("sem_pl4_ex1_read",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		 perror("Error in sem_open()");
		 exit(1);
	 }
	  if((sem_write = sem_open("sem_pl4_ex1_write",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		 perror("Error in sem_open()");
		 exit(1);
	 }
	 	
	 int i,j,number;
	 
	for(i = 0;i < NUMBER_OF_CHILDREN;i++){  
		if(fork() == 0){
			
		FILE *in_file  = fopen("Numbers.txt", "r"); 
		FILE *out_file = fopen("Output.txt", "a"); 
     
		if (in_file == NULL || out_file == NULL){ 
			printf("Error! Could not open file\n"); 
			exit(-1);
		} 	
		for(j = 0;j < 200;j++){
			sem_wait(sem_read);
			fscanf(in_file,"%d ",&number);
			sem_post(sem_read);
			sem_wait(sem_write);
			fprintf(out_file,"%d ",number);
			sem_post(sem_write); 
		}	
			fclose(out_file);
			fclose(in_file);
			exit(0);
		}	
	}
	
	for(i = 0;i < NUMBER_OF_CHILDREN;i++){
		wait(NULL);
	}

	FILE *out_file = fopen("Output.txt", "r"); 
     
	if (out_file == NULL){ 
		printf("Error! Could not open file\n"); 
		exit(-1);
	} 	
		
	while(fscanf(out_file,"%d",&number) == 1){
		printf("%d ",number);
	}
	fclose(out_file);
	
	sem_unlink("sem_pl4_ex1_read");
	sem_unlink("sem_pl4_ex1_write");	
	
	return 0;	 	 
}
	
	
	
	
	
	
	
