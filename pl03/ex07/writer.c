#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

int main() {
	
    int numbers[10],i;
    
    time_t t;
	srand(((unsigned)time(&t))*(getpid()));
    
    for (i = 0; i < 10; i++){
		numbers[i] = (rand() % 20) + 1;
	}
	
	int size = sizeof(int) * 10;
    int fd = shm_open("/pl3_ex7_shm1", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    
    ftruncate(fd, size);

    int *shared_data = (int*)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < 10; i++){
		shared_data[i] = numbers[i];
	}

    if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error

    return EXIT_SUCCESS;
}
