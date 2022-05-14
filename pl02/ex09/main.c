#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 50000
#define NUMBER_OF_CHILDREN 10
#define READ 0
#define WRITE 1

typedef struct {
 int customer_code;
 int product_code;
 int quantity;
} Sale;

int alreadyExists(int code,int size,int array[]){
	
	int i;
	
	for(i = 0; i < size;i++){
		if (array[i] == code) return 1;
		
	}
	return 0;
}

int main(){

	int i,j,k=0,pipes[NUMBER_OF_CHILDREN][2];
	int products[50];
	Sale sales[ARRAY_SIZE];			
	time_t t;
	srand ((unsigned) time (&t));
	
	for(i = 0; i < NUMBER_OF_CHILDREN; i++){
		if(pipe(pipes[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i = 0;i < ARRAY_SIZE;i++){
		Sale tmp;
		tmp.customer_code = i;
		tmp.product_code = rand() % 50;
		tmp.quantity = 2;
		tmp.quantity = rand() % 22;
		sales[i] = tmp;
	}

	for(i = 0;i < NUMBER_OF_CHILDREN;i++){
		
		if(fork() == 0){
			close(pipes[i][READ]);
			
			for(j = i * ARRAY_SIZE/NUMBER_OF_CHILDREN; j < ((i+1) * ARRAY_SIZE/NUMBER_OF_CHILDREN);j++){
				if(sales[j].quantity > 20){
					write(pipes[i][WRITE],&sales[j].product_code,sizeof(int));
				}
			}	
			close(pipes[i][WRITE]);
			exit(0);		
		}
	}
		
	for(i = 0; i < NUMBER_OF_CHILDREN;i++){
		int code;

		close(pipes[i][WRITE]);
		while(read(pipes[i][READ],&code,sizeof(int)) > 0){
			if(alreadyExists(code,50,products) == 0){
				products[k] = code;
				k++;
			}
		}
		close(pipes[i][READ]);
	}
	
	printf("Products that sold more than 20 copies \n");
	for(i = 0; i < k; i++){
		printf("%d \n",products[i]);
	}
	
}

