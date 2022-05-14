#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
	printf("Initiating program 'CMD2' - Going to sleep for 2 seconds! \n");
	sleep(2);
	printf("'CMD2' finished ! \n");
}
