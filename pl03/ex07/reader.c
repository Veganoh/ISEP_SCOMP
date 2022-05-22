#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

int main() {
	
	int size = sizeof(int)* 10,i;
    int fd = shm_open("/pl3_ex7_shm1", O_RDWR, S_IRUSR|S_IWUSR);
    
    ftruncate(fd, size);

    int *shared_data = (int *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    int total = 0;

     for (i = 0; i < 10; i++){
		total += shared_data[i];
		printf("%d ", shared_data[i]);
	}
	printf("\nAverage Code  : %d\n", total/10);

    if (munmap(shared_data,size) < 0) exit(1);     // Disconnects shared memory and check for error
	if (close(fd) < 0) exit(1);					   // Close FD and check for error
	if (shm_unlink("/pl3_ex7_shm1") < 0) exit(1); // Unlink shared memory and check for error


    return EXIT_SUCCESS;
}
