#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_CHILDREN 10

typedef struct {
    int number[10];
} shared_data_type;


void wait_for_children(int n);

int spawn_children(int n);

int main(){
	
	int array[1000],j,i,max;
	time_t t;
    srand((unsigned)time(&t));
    
    for(i = 0; i < 1000; i++){
		array[i] = (rand() % 1000);
	}
	
	int size = sizeof(shared_data_type);
	int fd = shm_open("/pl3_ex9_shm1", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, size);
	shared_data_type *shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	int id = spawn_children(NUMBER_OF_CHILDREN);
	
	if(id > 0){
		max = 0;
			
		for (j = (id-1) * 100; j < (id) * 100; j++){
			if(max < array[j]) max = array[j];
		}
			
		printf("[NUMBER %d] %d\n",(id-1), max);
		shared_data->number[(id-1)] = max;

		exit(EXIT_SUCCESS);
	}
    
	wait_for_children(NUMBER_OF_CHILDREN);
	
	max = 0;
	for(i = 0; i < 10; i++){
		if(max < shared_data->number[i])
				max = shared_data->number[i];
	} 
	
	printf("[BIGGEST NUMBER] %d\n", max);
		
    if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error
	if (shm_unlink("/pl3_ex9_shm1") < 0) exit(1); // Unlink shared memory and check for error

 
	exit(EXIT_SUCCESS);
}

void wait_for_children(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}

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
