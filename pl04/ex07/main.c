#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

int NUMBER_OF_CHILDREN = 3;

int spawn_childs(int n){
	pid_t pid;
    int i;
    for (i = 0; i < n; i++){
		pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

void wait_for_childs(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}
	

int main(){

	/**
	 * SEMPAHORE CREATION
	 * */
	sem_t *sem1,*sem2,*sem3;
	if((sem1 = sem_open("pl4_ex7_sem1",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	if((sem2 = sem_open("pl4_ex7_sem2",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	if((sem3 = sem_open("pl4_ex7_sem3",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	int id = spawn_childs(NUMBER_OF_CHILDREN);
	switch(id){
		case 1:
			printf("Sistemas ");
			fflush(stdout);
			sem_post(sem2);
			sem_wait(sem1);
			printf("a ");
			fflush(stdout);
			sem_post(sem2);
			exit(EXIT_SUCCESS);
		break;
		
		case 2:
			sem_wait(sem2);
			printf("de ");
			fflush(stdout);
			sem_post(sem3);
			sem_wait(sem2);
			printf("melhor ");
			fflush(stdout);
			sem_post(sem3);
			exit(EXIT_SUCCESS);
		break;
		
		case 3:
			sem_wait(sem3);
			printf("Computadores - ");
			fflush(stdout);
			sem_post(sem1);
			sem_wait(sem3);
			printf("disciplina!\n");
			fflush(stdout);
			exit(EXIT_SUCCESS);
		break;
	}
	
	wait_for_childs(NUMBER_OF_CHILDREN);
		
	/**
	 *  UNLINKING SEMPAPHORES AND CHECKING FOR ERROR
	 * */
	
	if (sem_close(sem1) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex7_sem1") == -1) exit(EXIT_FAILURE);
	
	if (sem_close(sem2) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex7_sem2") == -1) exit(EXIT_FAILURE); 
	
	if (sem_close(sem3) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex7_sem3") == -1) exit(EXIT_FAILURE); 
	
	return EXIT_SUCCESS;
}
