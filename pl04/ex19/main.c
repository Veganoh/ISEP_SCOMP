/**
 * Para este exercício serão usados três semáforos, um deles tem como objetivo,
 * permitir apenas que os sensores avancem quando os semáforos e a memória partilhada tiverem criados
 * Os outros dois um deles é de exclusão mútua para acedar à memória partilhada e o outro é para avisar  
 * o controlador que pode ler mais um número
 * */

#include "utils.h"

#define NUMBER_OF_CHILDREN 6

int main(){
	
	int i,id,fd;
	shared_data_type *shared_data;

	sem_t *sem1,*sem2,*sem3;
	sem1 = sem_open("pl4_ex19_creation_done",O_CREAT|O_EXCL,0644,0);
	
	
	// SENSORS 
	id = spawn_children(NUMBER_OF_CHILDREN);
	if(id >= 1 && id < NUMBER_OF_CHILDREN){

		sem_wait(sem1);
		sem_post(sem1);
		
		shared_data = open_shared_memory("/pl4_ex19_shm1",&fd,O_CREAT|O_RDWR);

		sem2 = sem_open("pl4_ex19_excl",0);
		sem3 = sem_open("pl4_ex19_no_data",0);
		
		for(i=0;i < 6;i++){
			
			int generated_value = random_number_between(0,100);			
			shared_data -> values[id-1][i] = generated_value;
		
			sem_wait(sem2);
			if(generated_value > 50){
				shared_data -> alarm_sensors++;
			}else{
				if(shared_data->last_read < 50 && shared_data->alarm_sensors != 0) shared_data->alarm_sensors--;	
			}
			shared_data->last_read = generated_value;
			shared_data -> next_sensor = id-1;
			shared_data -> next_value = i;
			sem_post(sem3);
			sleep(1);
		}
		exit(EXIT_SUCCESS);
	}
	// CONTROLLER
	if(id == NUMBER_OF_CHILDREN){
		
		/**
		* SHARED MEMORY CREATION
		* */
		shared_data = open_shared_memory("/pl4_ex19_shm1",&fd,O_CREAT|O_EXCL|O_RDWR);
		shared_data->alarm_sensors = 0;
		shared_data->last_read = 100;

	
		/**
		* SEMAPHORE CREATION
		* */
		sem2 = sem_open("pl4_ex19_excl",O_CREAT|O_EXCL,0644,1);
		sem3 = sem_open("pl4_ex19_no_data",O_CREAT|O_EXCL,0644,0);
		
		sem_post(sem1);
		
		int last_number_alert_sensors = 0;
		
		for(i = 0;i < (NUMBER_OF_CHILDREN-1)*6;i++){
			sem_wait(sem3);
			printf("The sensor %d just read the value %d\n",shared_data->next_sensor+1,shared_data->values[shared_data->next_sensor][shared_data->next_value]);
			if(shared_data->alarm_sensors != last_number_alert_sensors){
				last_number_alert_sensors = shared_data->alarm_sensors;
				printf("The number of alert sensors is now: %d\n",last_number_alert_sensors);
			}
			sem_post(sem2);			
		}
	
		unlink_semaphore("pl4_ex19_creation_done");
		unlink_semaphore("pl4_ex19_excl");
		unlink_semaphore("pl4_ex19_no_data");

		close_shared_memory(shared_data,fd);
		unlink_shared_memory("/pl4_ex19_shm1");
		exit(EXIT_SUCCESS);
	}
	
	wait_for_children(NUMBER_OF_CHILDREN);
	exit(EXIT_SUCCESS);
}
