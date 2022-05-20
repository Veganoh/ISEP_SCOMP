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

int main(){
	
	int i,fd;
	

	
	if((fd = shm_open("/ex_mod_shm1",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR)) == -1){
       perror("Error at shm_open()!\n");
       exit(EXIT_FAILURE);
    }
    
    ftruncate(fd,sizeof(shared_data_type));
    shared_data_type *shared_data = (shared_data_type*)mmap(NULL,sizeof(shared_data_type),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	

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
	
	sem_wait(sems[1]);										// Espera que a votação comece
	sem_post(sems[1]);										// Se começou deixa os próximos entrarem

	if(sem_trywait(sems[2]) == -1){
		printf("Já foram excedidos os 500 lugares\n");
		exit(EXIT_SUCCESS);
	}
	
	time_t t;
	srand(((unsigned)time(&t))*(getpid()));
	sem_wait(sems[0]);
	printf("%s",shared_data->mocao);
	int number = (rand() % 2) + 1;
	
	if(number == 1){
		printf("[%d] I'm gonna vote Y \n",getpid());
		shared_data ->votos[shared_data->next] = 'Y';
	}else{
		printf("[%d] I'm gonna vote N \n",getpid());
		shared_data ->votos[shared_data->next] = 'N';
	}
	shared_data ->next++;
	shared_data ->nmr_votos++;
	sem_post(sems[0]);
	exit(EXIT_SUCCESS);
}
