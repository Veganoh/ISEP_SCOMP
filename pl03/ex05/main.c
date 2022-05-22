#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef struct {
	int a;
	int b;
} shared_data_type;

int main(){
	
	int fd,r,i,data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	
	fd = shm_open("/pl3ex5",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	
	ftruncate(fd,data_size);
	shared_data = (shared_data_type*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	shared_data ->a = 8000;
	shared_data ->b = 200;
		
	if (fork() == 0){
		for(i = 0;i < 1000000;i++){
			shared_data -> b--;
			shared_data -> a++;
		}
		exit(0);
		
	}else{
		for(i = 0;i < 1000000;i++){
			shared_data -> b++;
			shared_data -> a--;
		}
	}
	printf("A = %d B = %d\n",shared_data->a,shared_data->b);	
	
	r = munmap(shared_data,data_size);
	if (r < 0) exit(1);
	
	r = shm_unlink("/pl3ex5");
	if (r < 0) exit(1);
		
}





