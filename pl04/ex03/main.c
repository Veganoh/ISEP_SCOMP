#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

int NUMBER_OF_CHILDREN = 50;

typedef struct{
	char strings[50][80];
	int next;
}shared_data_type;

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

int main(){
	
	/**
	 * SHARED MEMORY 
	 * */
	int size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	int fd = shm_open("/pl4_ex3_shm1",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_data = (shared_data_type*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	
	shared_data -> next = 0;

	/**
	 * SEMPAHORE CREATION
	 * */
	sem_t *sem;
	if((sem = sem_open("pl4_ex3_sem1",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	 	 
	int id = spawn_childs(NUMBER_OF_CHILDREN);
	if(id > 0){			// If child
		sem_wait(sem);
		sprintf(shared_data->strings[shared_data->next],"ID [%d] - PID[%d]",id,getpid());
		sleep(1);
        shared_data->next++;
		sem_post(sem); 
		exit(EXIT_SUCCESS);
	 }
	 
	 
	int i;
	for(i = 0; i < NUMBER_OF_CHILDREN;i++){ // Wait for all children
		 wait(NULL);
	}
	for (i = 0; i < 50; i++){
        printf("%s\n", shared_data->strings[i]);
	}
	
	if (munmap(shared_data,size) < 0) exit(1);    // Disconnects shared memory and check for error
	
	if (close(fd) < 0) exit(1);					  // Close FD and check for error

	if (shm_unlink("/pl4_ex3_shm1") < 0) exit(1); // Unlink shared memory and check for error
	 
	if (sem_unlink("pl4_ex3_sem1") < 0) exit(1); // Unlink semaphore and check for error
	
	return 0;
}
