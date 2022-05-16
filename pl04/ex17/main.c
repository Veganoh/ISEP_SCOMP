/**
 * 
 * Para este exercício serão usados três semáforos, um deles tem como objetivo,
 * controlar o acesso a recursos com capacidade limitada
 * neste caso 300 que é a capacidade máxima do clube.
 * Os outros dois serão torniquetes que irão a segurar a prioridade dos VIP sobre todos 
 * e dos especiais sobre os normais.
 * */


#include "utils.h"

#define NUMBER_OF_SEMAPHORES 3
#define ROOM_CAPACITY 300
#define NUMBER_OF_VIPS 100
#define NUMBER_OF_SPECIALS 200 
#define NUMBER_OF_NORMALS 300

int main(){
	
	int i,id,sleep_time;
	
	/**
	 * SEMAPHORES CREATION 
	 * */
	char SEM_NAMES[NUMBER_OF_SEMAPHORES][80] = {"pl4_ex17_sem_vip","pl4_ex17_sem_special","pl4_ex17_sem_room"};
	int SEM_VALUES[NUMBER_OF_SEMAPHORES] = {1,1,ROOM_CAPACITY};

	sem_t *sems[NUMBER_OF_SEMAPHORES];	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		if((sems[i] = sem_open(SEM_NAMES[i],O_CREAT|O_EXCL,0644,SEM_VALUES[i])) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(EXIT_FAILURE);
		}	
	}
	
	/**
	 * VIPS 
	 * */
	id = spawn_children(NUMBER_OF_VIPS);
	if(id > 0){
		sem_wait(sems[0]);
		sem_wait(sems[2]);
		sleep_time = random_number_between(1,60);
		printf("[VIP - %d] - [ENTERED THE CLUB] \n",id);
		sem_post(sems[0]);
		sleep(sleep_time);
		sem_post(sems[2]);
		printf("[VIP - %d] - [LEFT THE CLUB] - [DURATION - %d] \n",id,sleep_time);
		exit(EXIT_SUCCESS);
	}
	
	
	/**
	 * SPECIALS 
	 * */
	id = spawn_children(NUMBER_OF_SPECIALS);
	if(id > 0){
		sem_wait(sems[0]);
		sem_post(sems[0]);
		
		sem_wait(sems[1]);
		sem_wait(sems[2]);
		sleep_time = random_number_between(1,60);
		printf("[SPECIAL - %d] - [ENTERED THE CLUB] \n",id);
		sem_post(sems[1]);
		sleep(sleep_time);
		sem_post(sems[2]);
		printf("[SPECIAL - %d] - [LEFT THE CLUB] - [DURATION - %d] \n",id,sleep_time);
		exit(EXIT_SUCCESS);
	}


	/**
	 * NORMALS 
	 * */
	id = spawn_children(NUMBER_OF_NORMALS);
	if(id > 0){
		sem_wait(sems[0]);
		sem_post(sems[0]);
		
		sem_wait(sems[1]);
		sem_post(sems[1]);
		
		sem_wait(sems[2]);
		sleep_time = random_number_between(1,60);
		printf("[NORMAL - %d] - [ENTERED THE CLUB] \n",id);
		sleep(sleep_time);
		sem_post(sems[2]);
		printf("[NORMAL - %d] - [LEFT THE CLUB] - [DURATION - %d] \n",id,sleep_time);
		exit(EXIT_SUCCESS);
	}
	
	wait_for_children(NUMBER_OF_NORMALS);
	wait_for_children(NUMBER_OF_SPECIALS);
	wait_for_children(NUMBER_OF_VIPS);
	
	for(i = 0;i < NUMBER_OF_SEMAPHORES;i++){
		close_semaphore(sems[i]);
		unlink_semaphore(SEM_NAMES[i]);
	}
	
	return 0;
}
