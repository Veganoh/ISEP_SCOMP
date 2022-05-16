#include "utils.h"

#define NUMBER_OF_CHILDREN 2
#define NUMBER_OF_SEMAPHORES 3
#define BUFFER_SIZE 10

typedef struct{
	int data[BUFFER_SIZE];
	int head;
	int tail;
}shared_data_type;

int main(){
	
	int i,id;
	
	/**
	 * SHARED MEMORY CREATION
	 * */
	int size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	int fd = shm_open("/pl4_ex13_shm1",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_data = (shared_data_type*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data -> head = 0;
	shared_data -> tail = 0;
	
	/**
	 * SEMAPHORES CREATION 
	 * */
	char SEM_NAMES[NUMBER_OF_SEMAPHORES][80] = {"pl4_ex13_sem_excl", "pl4_ex13_items", "pl4_ex13_space"};
	int SEM_VALUES[NUMBER_OF_SEMAPHORES] = {1,0,BUFFER_SIZE};
	sem_t *sems[NUMBER_OF_SEMAPHORES];	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		if((sems[i] = sem_open(SEM_NAMES[i],O_CREAT|O_EXCL,0644,SEM_VALUES[i])) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(EXIT_FAILURE);
		}	
	}
	
	id = spawn_children(NUMBER_OF_CHILDREN);
	//CHILDREN
	if(id > 0){
		for(i = 0;i < 15;i++){
			sem_wait(sems[2]);
			sem_wait(sems[0]);
			shared_data -> data[shared_data->head] = random_number_between(1,9);
			
			if(shared_data->head == BUFFER_SIZE - 1){
				shared_data->head = 0;
			}else{
				shared_data->head++;
			}
			sem_post(sems[0]);
			sem_post(sems[1]);	
			sleep(1);             // This line is not necessary
		}
	exit(EXIT_SUCCESS);
	//FATHER
	}else{
		for(i = 0;i < 30;i++){
			sem_wait(sems[1]);
			sem_wait(sems[0]);
			printf("I just read a new number: %d \n",shared_data->data[shared_data->tail]);
			
			if(shared_data->tail == BUFFER_SIZE - 1){
				shared_data->tail = 0;
			}else{
				shared_data->tail++;
			}
			sem_post(sems[0]);
			sem_post(sems[2]);	
		}
	}
	
	wait_for_children(NUMBER_OF_CHILDREN);
	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		close_semaphore(sems[i]);
		unlink_semaphore(SEM_NAMES[i]);
	}
    
    if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error
	if (shm_unlink("/pl4_ex13_shm1") < 0) exit(1); // Unlink shared memory and check for error
}

