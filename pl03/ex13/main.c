#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUMBER_OF_CHILDREN 10

typedef struct{
	char path[20];
	char word[10][10];
	int occurrences[10];
} shared_data_type;

int spawn_children(int n);

int main(){
	
	int size = sizeof(shared_data_type);
	int j;
	int fd = shm_open("/pl3_ex13_shm1", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd, size);
	shared_data_type *shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	strcpy(shared_data->path, "text.txt");
	strcpy(shared_data->word[0], "wonderful");
	strcpy(shared_data->word[1], "when");
	strcpy(shared_data->word[2], "then");
	strcpy(shared_data->word[3], "that");
	strcpy(shared_data->word[4], "friend");
	strcpy(shared_data->word[5], "was");
	strcpy(shared_data->word[6], "this");
	strcpy(shared_data->word[7], "is");
	strcpy(shared_data->word[8], "the");
	strcpy(shared_data->word[9], "in");
	
	
	int id = spawn_children(NUMBER_OF_CHILDREN);
		
	if(id > 0){
			
		FILE *fp = fopen(shared_data->path,"r");

		char buf[20];
		int appereances = 0;

		while(fscanf(fp, "%s", buf) != E){
			for(j = 0; buf[j]; j++){
				buf[j] = tolower(buf[j]);
			}
			if(strcmp(buf, shared_data->word[id-1]) == 0)
				appereances++;
		}
		shared_data->occurrences[id-1] = appereances;
		exit(EXIT_SUCCESS);
	}
	
    
    for (int i = 0; i < 10; i++){
		wait(NULL);
	}
		
	for (int i = 0; i < 10; i++){
		printf("[%s] appeared %d times\n", shared_data->word[i], shared_data->occurrences[i]);	
	}

	if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error
	if (shm_unlink("/pl3_ex13_shm1") < 0) exit(1); // Unlink shared memory and check for error

	exit(EXIT_SUCCESS);
}

int spawn_children(int n){
	pid_t pid;
    int i;
    for (i = 0; i < n; i++){
		pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i +1;
    }
    return 0;
}
