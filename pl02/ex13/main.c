#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main(void){
	int i,numMachines = 4,numPieces= 1000;
	int piecesReceived, notification;
	int notificationPipes[4][2],transferPipes[4][2];
	int piecesToBePassed[4] = {5,5,10,100};
	const int sent = 1,finished = 0;						// Flags
	
	for(i = 0; i < numMachines; i++){
		if(pipe(notificationPipes[i]) == -1 || pipe(transferPipes[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	printf("Starting the production of %d pieces! \n",numPieces);
	for(i = 0; i < numMachines; i++){
		
		if(fork() == 0){
			close(transferPipes[i][READ]);
			close(notificationPipes[i][READ]);
			if(i == 0){																						// Machine 1 has a different process than the other so it has to be 
				while(numPieces > 0){																		// worked separately from the others.
					write(transferPipes[i][WRITE],&piecesToBePassed[i],sizeof(piecesToBePassed[i]));
					numPieces -= 5;
					write(notificationPipes[i][WRITE],&sent,sizeof(sent));
					printf("5 pieces were sent from M1 to M2! \n");
				}
			}else{
				int piecesToBeSent = 0;
				read(notificationPipes[i-1][READ], &notification, sizeof(notification));
				while(notification != 0){
					if(notification == 1){
						read(transferPipes[i-1][READ],&piecesReceived, sizeof(int));
						piecesToBeSent += piecesReceived;
					
						if(piecesToBeSent == piecesToBePassed[i]){
							write(transferPipes[i][WRITE], &piecesToBePassed[i], sizeof(piecesToBePassed[i]));
							write(notificationPipes[i][WRITE], &sent, sizeof(sent));
							piecesToBeSent = 0;

							if(i != numMachines){
								printf("%d pieces were sent from  M%d to M%d! \n", piecesToBePassed[i], i+1, i+2);
							}
							else{
								printf("%d pieces were sent from  M%d to  A1 and inventory! \n", piecesToBePassed[i], i+1);
							}
						}
					}	
					read(notificationPipes[i-1][READ], &notification, sizeof(notification));
				}
			}		
			write(notificationPipes[i][WRITE], &finished, sizeof(finished));
			close(transferPipes[i][WRITE]);
			close(notificationPipes[i][WRITE]);	
			exit(EXIT_SUCCESS);	
		}
	}
	int storageA = 0,inventory = 0;
	read(notificationPipes[3][READ], &notification, sizeof(notification));
	
	while(notification != 0){
		if(notification == 1){
			read(transferPipes[3][READ], &piecesReceived, sizeof(piecesReceived));
			if(piecesReceived == piecesToBePassed[3]){
				storageA += piecesReceived;
				inventory += piecesReceived;
			}
		}
		read(notificationPipes[3][READ], &notification, sizeof(notification));
	}
	close(notificationPipes[3][WRITE]);
	close(transferPipes[3][WRITE]);
	printf("Pieces in storage A - %d ||| Pieces in inventory - %d \n", storageA, inventory);
	return 0;
}
