#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
	
	int x = 1;
	pid_t p = fork();
	
	if (p == 0) {
		x = x+1;
		printf("1. x = %d\n", x);
	} else {
		x = x-1;
		printf("2. x = %d\n", x);
	}
	printf("3. %d; x = %d\n", p, x);
} 


/**
 * Even though we can not define a concrete output because the son may be faster than its dad
 * but one possible outcome is:
 * 
 * 2. x = 0
 * 3. 1234; x = 0
 * 1. x = 2
 * 3. 0; x = 2
 */

/** 
 a) The order of the output can change however because the processes will execute according with the available resources, so the parent process can take longer than the child, or the other way around. 
	When it comes to the value of 'x', since it's a fork it will start with the same value in both of the proccesses, what will change is the value of 'p' which in the parent process it will have the 
PDI of the child so it will be '1234' and the child process will have the value of '0' since this allows us to know which process is the child and which one is the parent.
	Having this in mind it's easy to identify the output of the code by analysing it.
 
 b) No you are not. Because the processes run in random order, the child process might be able to print the line 11 before the parent process is able to get to the line 9 therefore the order of the printf() are not garanteed.
*/
 

