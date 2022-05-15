#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

int main(){
	
	/**
	 * SEMPAHORE CREATION
	 * */
	sem_t *sem;
	if((sem = sem_open("pl4_ex5_sem1",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	if(fork() == 0){
		printf("I'm the child \n");
		sem_post(sem);
		exit(EXIT_SUCCESS);
	}else{
		sem_wait(sem);
		printf("I'm the father \n");
	}
	
	if (sem_close(sem) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex5_sem1") == -1) exit(EXIT_FAILURE); // Unlink semaphore and check for error
	
	return EXIT_SUCCESS;
}

