#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
	printf("Initiating program 'CMD3' - Going to sleep for 10 seconds! \n");
	sleep(10);
	printf("'CMD3' finished ! \n");
}
