#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() 
{
    int i,j,inicio,fim;

    for(i = 0; i < 10; i++){
		
        if(fork() == 0){
			printf("100 Números: \n");
			
           inicio = 100*i+1;
           fim = 100*(i+1);
           
            for(j = inicio; j < fim; j++){
                printf("%d ", j);
            }
            
            printf("\n");
            exit(0);
        }
    }
    for(i=0; i<10; i++){
        wait(NULL);
    }
}

/*
	O output não vai ser sorted porque não há como saber qual processo irá decorrer primeiro, só se sabe que os filhos vão acabar primeiro do que o pai
que este irá esperar pelo termino de cada um!
 
*/


