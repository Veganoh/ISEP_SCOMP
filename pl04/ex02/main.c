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
	
	int i,j,number;

	/**
	 * CREATE SEMAPHORES
	 * */
	sem_t *sems[NUMBER_OF_CHILDREN];
	char name[15];
	for(i = 1;i < NUMBER_OF_CHILDREN;i++){
		sprintf(name,"pl4_ex2_sem%d",i);
		if((sems[i] = sem_open(name,O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(1);
		}		
	}
	if((sems[0] = sem_open("pl4_ex2_sem0",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	for(i = 0;i < NUMBER_OF_CHILDREN;i++){  
		if(fork() == 0){
		
		sem_wait(sems[i]);
		FILE *in_file  = fopen("Numbers.txt", "r"); 
		FILE *out_file = fopen("Output.txt", "a"); 
     
		if (in_file == NULL || out_file == NULL){ 
			printf("Error! Could not open file\n"); 
			exit(-1);
		} 	
		for(j = 0;j < 200;j++){
			fscanf(in_file,"%d ",&number);
			fprintf(out_file,"%d ",number);
		}	
		fclose(out_file);
		fclose(in_file);
		sem_post(sems[i+1]);
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
	printf("\n");
	
	fclose(out_file);	
	
	for(i = 0;i < NUMBER_OF_CHILDREN;i++){
		sprintf(name,"pl4_ex2_sem%d",i);
		sem_unlink(name);	
	}	
	
	
	return 0;	 	 
}
	
	
	
