/**
 * Para este exercício serão usados quatro semáforos, um para cada sala
 * como o lobby não tem quaisquer regras considera-se que se uma pessoa
 * estiver a tentar entrar numa sala está automaticamente no lobby à espera.
 * Os semáforos vão ter como objetivo, controlar o acesso a recursos com capacidade
 * limitada com valor inicial de 5 que é a lotação máxima de cada sala.
 * Será tambémm necessário mais um semáforo para que seja possível apenas entrar e sair
 * pessoas apenas quando começar ou acabar um show, como todos os shows começam ao mesmo tempo
 * não é necessário criar um semáforo para cada sala mas caso contrário era necessário criar um para cada
 * sala diferente.Este semáforo é para sincronização de processos com eventos.
 * */


#include "utils.h"

#define NUMBER_OF_CHILDREN 26
#define NUMBER_OF_SEMAPHORES 5
#define ROOM_CAPACITY 5
#define NUMBER_OF_SHOWS 2
#define SHOW_DURATION 5

int main(){
	
	int i,id;
	
	/**
	 * SEMAPHORES CREATION 
	 * */
	char SEM_NAMES[NUMBER_OF_SEMAPHORES][80] = {"pl4_ex15_sem_excl","pl4_ex15_sem_room1","pl4_ex15_sem_room2", "pl4_ex15_sem_room3","pl4_ex15_sem_room4"};
	int SEM_VALUES[NUMBER_OF_SEMAPHORES] = {0,ROOM_CAPACITY,ROOM_CAPACITY,ROOM_CAPACITY,ROOM_CAPACITY};

	sem_t *sems[NUMBER_OF_SEMAPHORES];	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		if((sems[i] = sem_open(SEM_NAMES[i],O_CREAT|O_EXCL,0644,SEM_VALUES[i])) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(EXIT_FAILURE);
		}	
	}
	
	id = spawn_children(NUMBER_OF_CHILDREN);
	//VISITOR
	if(id >= 1 && id <= 25){
		int show_atending = random_number_between(1,4);
		
		while(1){
			if(sem_trywait(sems[show_atending]) == 0){                 // Tries to enter the room, only if there is space and  if it is the appropriate time to do it
				if(sem_trywait(sems[0]) == 0){    
					sem_post(sems[0]);
					printf("%d just entered the room %d \n",id,show_atending);
					sleep(SHOW_DURATION);
					sem_wait(sems[0]);
					sem_post(sems[0]);
					sem_post(sems[show_atending]);
					printf("%d left the room %d \n",id,show_atending);
					break;
				}else{
					sem_post(sems[show_atending]);
				}
			}
			
		}
		exit(EXIT_SUCCESS);	
	}
	// SHOW ROOM CONTROLLING AREA
	if(id == 26){
			printf("DOORS OPENED\n");
			sem_post(sems[0]);
		for(i = 1;i <= NUMBER_OF_SHOWS;i++){
			sleep(1);
			printf("SHOW %d - STARTING, DOORS CLOSED\n",i);
			sem_wait(sems[0]);
			sleep(5);
			printf("SHOW %d - ENDED, DOORS OPENED\n",i);
			sem_post(sems[0]);
			if(NUMBER_OF_SHOWS == i){
				sleep(1);
				sem_wait(sems[0]);
				printf("THE SHOWS ENDED FOR TODAY,THANKS FOR YOUR COMPANY!\n");
			}
		}
		exit(EXIT_SUCCESS);
	} 
	
	wait_for_children(NUMBER_OF_CHILDREN);
	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		close_semaphore(sems[i]);
		unlink_semaphore(SEM_NAMES[i]);
	}
	
	return 0;
}
