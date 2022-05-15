#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h> 
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

#define NUMBER_OF_CHILDREN 10

typedef struct{
	int nproc_at_barrier;
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

void wait_for_childs(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}

void buy_chips(){
	printf("[PID-%d] Buying chips...\n",getpid());
}

void buy_beer(){
	printf("[PID-%d] Buying beer...\n",getpid());
}

void eat_and_drink(){
	printf("[PID-%d] Eating and drinking... \n",getpid());
}

void exec_buy(int option){
	if (option == 1) buy_chips();
	else buy_beer();
}

int main(){
	
	/**
	 * SHARED MEMORY CREATION
	 * */
	int size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	int fd = shm_open("/pl4_ex9_shm1",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_data = (shared_data_type*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data -> nproc_at_barrier = 0;
	
	/**
	 * SEMPAHORE CREATION
	 * */
	sem_t *sem1,*sem2;
	if((sem1 = sem_open("pl4_ex9_sem1",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	if((sem2 = sem_open("pl4_ex9_sem2",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(EXIT_FAILURE);
	}
	
	int id = spawn_childs(NUMBER_OF_CHILDREN);
	if(id > 0){
		srand (getpid());
		int option = (rand () % 2)+1;
		sleep((rand()%5)+1);
		exec_buy(option);
		sem_wait(sem1);
		shared_data -> nproc_at_barrier++;
		sem_post(sem1);
		if(shared_data -> nproc_at_barrier == NUMBER_OF_CHILDREN) sem_post(sem2);
		sem_wait(sem2);
		eat_and_drink();
		sem_post(sem2);
		exit(EXIT_SUCCESS);
	}
	
	wait_for_childs(NUMBER_OF_CHILDREN);
	
	
	/**
	 *  UNLINKING SEMPAPHORES AND CHECKING FOR ERROR
	 * */
	
	if (sem_close(sem1) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex9_sem1") == -1) exit(EXIT_FAILURE);
	
	if (sem_close(sem2) == -1) exit(EXIT_FAILURE);
	if (sem_unlink("pl4_ex9_sem2") == -1) exit(EXIT_FAILURE); 
	
	if (munmap(shared_data,size) < 0) exit(1);    // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					  // Close FD and check for error
	if (shm_unlink("/pl4_ex9_shm1") < 0) exit(1); // Unlink shared memory and check for error
	
	return EXIT_SUCCESS;
}
