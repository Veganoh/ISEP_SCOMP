#include "utils.h"

#define NUMBER_OF_SEMAPHORES 5
#define NUMBER_OF_CHILDREN 3
#define TRAIN_SIZE 200

typedef struct{
	int entering;
	int leaving;
}shared_data_type;

int main(){
	
	int i,id,quantity;

	/**
	 * SHARED MEMORY CREATION
	 * */
	int size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	int fd = shm_open("/pl4_ex11_shm1",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,size);
	shared_data = (shared_data_type*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data -> entering = 250;
	shared_data -> leaving = 250;
	
	/**
	 * SEMAPHORES CREATION 
	 * */
	char SEM_NAMES[NUMBER_OF_SEMAPHORES][80] = {"pl4_ex11_sem_passengers", "pl4_ex11_sem1", "pl4_ex11_sem2", "pl4_ex11_sem3","pl4_ex11_sem4"};
	int SEM_VALUES[NUMBER_OF_SEMAPHORES] = {200,1,1,1,1};
	sem_t *sems[NUMBER_OF_SEMAPHORES];	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		if((sems[i] = sem_open(SEM_NAMES[i],O_CREAT|O_EXCL,0644,SEM_VALUES[i])) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(EXIT_FAILURE);
		}	
	}
	
	id = spawn_children(NUMBER_OF_CHILDREN);
	if(id > 0){
		while(shared_data -> entering != 0 ||shared_data -> leaving != 0){
			srand(getpid());
			int in_out = (rand() % 2)+1;            // 1 if passenger is entering, 2 it is leaving
			
			sem_wait(sems[4]);
			sem_getvalue(sems[0],&quantity);
			if(shared_data-> leaving == 0 ||quantity == 200){
				in_out = 1;
			}
			if(shared_data-> entering == 0 ||quantity == 0){
				in_out = 2;
			}
			sem_post(sems[4]);

		
			sem_wait(sems[id]);
			if(in_out == 1){
				sem_wait(sems[4]);
				shared_data->entering--;
				sem_post(sems[4]);
				sem_wait(sems[0]);
				printf("A passenger has entered in door %d\n",id);
				sleep(0);
				sem_post(sems[id]);
			}else{
				sem_wait(sems[4]);
				shared_data->leaving--;
				sem_post(sems[4]);
				sem_post(sems[0]);
				printf("A passenger has left in door %d\n",id);
				sleep(0);
				sem_post(sems[id]);
			}
		
		}
		exit(EXIT_SUCCESS);
	}
	
	wait_for_children(NUMBER_OF_CHILDREN);
	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		close_semaphore(sems[i]);
		unlink_semaphore(SEM_NAMES[i]);
	}
    
    if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error
	if (shm_unlink("/pl4_ex11_shm1") < 0) exit(1); // Unlink shared memory and check for error

	return 0;
}
