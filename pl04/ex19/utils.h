#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
	int values[5][6];
	int next_sensor;
	int next_value;
	int last_read;
	int alarm_sensors;
} shared_data_type;

/**
 * SHARED UTILS
 * */
shared_data_type *open_shared_memory(char *shm_name, int *fd, int oflag) {
    
    *fd = shm_open(shm_name,oflag,S_IRUSR|S_IWUSR);
    if (*fd == -1) {
        perror("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }
    
    ftruncate(*fd,sizeof(shared_data_type));

    shared_data_type *sharedData = (shared_data_type*)mmap(NULL,sizeof(shared_data_type),PROT_READ|PROT_WRITE,MAP_SHARED,*fd, 0);

    return sharedData;
}

void close_shared_memory(shared_data_type *shared_data, int fd){
    
    if (munmap((void *)shared_data, sizeof(shared_data_type)) < 0){ 		// Disconnects shared memory and check for error
        perror("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(fd) < 0){													// Close FD and check for error
        perror("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
}

void unlink_shared_memory(char* shm_name){									// Unlink shared memory and check for error
    if (shm_unlink(shm_name) < 0){
        perror("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }
}
	
/**
 * SEMAPHORES UTILS
 * */
 
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

/**
 * PROCESSES UTILS
 * */
int spawn_children(int n){
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

void wait_for_children(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}

int random_number_between(int min,int max){
	time_t t;
	srand(((unsigned)time(&t))*(getpid()));
	int number = (rand() % max) + min;
	return number;
}

