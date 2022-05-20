#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_SEMAPHORES 3

typedef struct {
	char mocao[80];
	char votos[500];
	int next;
	int nmr_votos;
	
} shared_data_type;


 void close_semaphore(sem_t *sem){
	if(sem_close(sem) == -1){
		perror("Error at sem_close()!\n");
		exit(EXIT_FAILURE);
    }
 }
 
 void unlink_semaphore(char* sem_name){
	if(sem_unlink(sem_name) == -1){
		perror("Error at sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }
 }
 
int main(){

	int fd,i;
	int votos_sim = 0, votos_nao = 0;
	
	
	if((fd = shm_open("/ex_mod_shm1",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR)) == -1){
       perror("Error at shm_open()!\n");
       exit(EXIT_FAILURE);
    }
    
    ftruncate(fd,sizeof(shared_data_type));
    shared_data_type *shared_data = (shared_data_type*)mmap(NULL,sizeof(shared_data_type),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    shared_data -> next = 0;
    shared_data -> nmr_votos = 0;
    
    
     /**
	 * SEMAPHORES CREATION 
	 * */
	char SEM_NAMES[NUMBER_OF_SEMAPHORES][80] = {"ex_mod_sem_excl","ex_mod_sem_start","ex_mod_sem_capacidade"};
	int SEM_VALUES[NUMBER_OF_SEMAPHORES] = {1,0,500};
	
	sem_t *sems[NUMBER_OF_SEMAPHORES];	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		if((sems[i] = sem_open(SEM_NAMES[i],O_CREAT,0644,SEM_VALUES[i])) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(EXIT_FAILURE);
		}	
	}
	
	
	sprintf(shared_data->mocao,"Mocao do candidato X");
	printf("A votação vai começar em 5 segundos\n");
	sleep(5);
	printf("A votação começou!\n");
	sem_post(sems[1]);
	sleep(20);
	printf("A votação irá acabar em 10 segundos!\n");
	sleep(10);
	sem_wait(sems[1]);
	printf("A votação acabou\n");	
	printf("A contar votos\n");
	sem_wait(sems[0]);
	
	for(i = 0;i < shared_data -> nmr_votos;i++){
		if(shared_data -> votos[i] == 'Y'){
			votos_sim++;
		}else votos_nao++;
	}
	printf("Os votos foram contados \n SIM - %d \n NÃO - %d \n",votos_sim,votos_nao); 

	if (munmap((void *)shared_data, sizeof(shared_data_type)) < 0){ 		// Disconnects shared memory and check for error
        perror("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(fd) < 0){													// Close FD and check for error
        perror("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
    
    if (shm_unlink("/ex_mod_shm1") < 0){
        perror("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		close_semaphore(sems[i]);
		unlink_semaphore(SEM_NAMES[i]);
	}
    
    
    
    exit(EXIT_SUCCESS);
}
