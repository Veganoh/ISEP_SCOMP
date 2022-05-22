#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct {
	char name[20];
	int number;
	int new_data;
} shared_data_type;

int main(){
	int fd,r,data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	
	fd = shm_open("/pl3ex1",O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	
	ftruncate(fd,data_size);
	shared_data = (shared_data_type*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	while(!shared_data->new_data);
	printf("The name is: %s\n",shared_data->name);
	printf("The number is: %d\n",shared_data->number);
	
	r = munmap(shared_data,data_size);
	if (r < 0) exit(1);
	
	r = shm_unlink("/pl3ex1");
	if (r < 0) exit(1);

	return 0;
}
