#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define READ 0
#define WRITE 0
#define ARRAY_SIZE 100000


typedef struct {
    int integer;
    char message[20];
} data_struct;

typedef struct {
    int last_integer;
    data_struct data[ARRAY_SIZE];
} shared_data_type;

void pipes(){
	
	clock_t t = clock();
    int fd[2],i;
    
	if(pipe(fd) == -1){
		perror("Pipe failed");
		exit(1);
	}
	
	data_struct data[ARRAY_SIZE],readStruct;
	
	if(fork() > 0){
		close(fd[READ]);
		
		for(i = 0; i < ARRAY_SIZE; i++){
			data[i].integer = i;
			strcpy(data[i].message, "ISEP – SCOMP 2020");
		}
			
		for(i = 0; i < ARRAY_SIZE; i++){
			write(fd[WRITE], &data[i], sizeof(data_struct));
		}
		close(fd[WRITE]);
	}else{
		close(fd[WRITE]);
		
		for(i = 0; i < ARRAY_SIZE; i++){
			read(fd[READ], &readStruct, sizeof(data_struct));
			data[i] = readStruct;
		}
		
		close(fd[READ]);
		exit(EXIT_SUCCESS);
	}
    
    wait(NULL);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("[PIPES %fs] \n", time_taken);	
}
void memory(){
	
	clock_t t;
    t = clock();
    
    int size = sizeof(shared_data_type),i,fd;
	data_struct data[ARRAY_SIZE];
	
	if(fork() > 0){
		fd = shm_open("/pl3_ex3_shm1",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd, size);
		shared_data_type *shm_data = (shared_data_type *)mmap(NULL,size,PROT_READ|PROT_WRITE, MAP_SHARED,fd, 0);
		
		for (i = 0; i < ARRAY_SIZE; i++){
			while(shm_data->last_integer < i);
			data[i] = shm_data->data[i];
		}
		wait(NULL);
		if (munmap(shm_data,size) < 0) exit(1);     	// Disconnects shared memory and check for error
		if (close(fd) < 0) exit(1);					   // Close FD and check for error
		if (shm_unlink("/pl3_ex3_shm1") < 0) exit(1); // Unlink shared memory and check for error
	} else {
		for (i = 0; i < ARRAY_SIZE; i++){
			data[i].integer = i;
			strcpy(data[i].message, "ISEP – SCOMP 2020");
		}
		fd = shm_open("/pl3_ex3_shm1",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
		ftruncate(fd, size);
		shared_data_type *shm_data = (shared_data_type*)mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

		for (i = 0; i < ARRAY_SIZE; i++){
			shm_data->data[i] = data[i];
			shm_data->last_integer = data[i].integer;
		}
		exit(EXIT_SUCCESS);
	}		
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("[MEMORY %fs] \n", time_taken);
}

int main(){
	pipes();
	memory();
}
