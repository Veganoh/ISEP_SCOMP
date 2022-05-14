#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
	printf("Initiating program 'CMD1' - Going to sleep for 5 seconds! \n");
	sleep(5);
	printf("'CMD1' finished ! \n");
}
