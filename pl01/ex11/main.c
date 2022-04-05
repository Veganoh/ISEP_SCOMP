#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

int ARRAY_SIZE = 1000;
int NUMBER_OF_CHILDREN = 5;

int main()
{
    int numbers[ARRAY_SIZE], result[ARRAY_SIZE];
	int i,j,status,max = INT_MIN;
    time_t t;
    srand((unsigned)time(&t));

    
    for (i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand() % (256);
    }
    
    for(i = 0;i < NUMBER_OF_CHILDREN;i++){
		if(fork() == 0){
			for(j = (ARRAY_SIZE/NUMBER_OF_CHILDREN)*i; j < (ARRAY_SIZE/NUMBER_OF_CHILDREN)*(i+1);j++){
				if(numbers[j] > max){
					max = numbers[j];
				}
			}
			exit(max);
		}
	}
    
    for (i = 0; i < NUMBER_OF_CHILDREN; i++){
        wait(&status);
        if (WIFEXITED(status)){
            if (max < WEXITSTATUS(status))
            {
                max = WEXITSTATUS(status);
            }
        }
    }
 
 
    if (fork() == 0){
        for (i = 0; i < ARRAY_SIZE/2; i++){
            result[i] = (int)((numbers[i]*100) / max);
			printf("%d - %d\n", i, result[i]);
        }
    }else{
		for (i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){  
			result[i] = (int)((numbers[i]*100) / max);
		}
		
		wait(NULL);
		
		for(i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
			printf("%d - %d\n", i, result[i]);
		}
    }
    return 0;
}
