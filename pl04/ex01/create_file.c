#include <stdio.h>
#include <stdlib.h>

int main(){

 FILE *out_file = fopen("Numbers.txt","w");
 
 if (out_file == NULL) {   
    printf("Error! Could not open file\n"); 
    exit(-1); // must include stdlib.h 
  } 
           
  int i = 0;
  
  for(i = 1; i <= 200;i++){
	  if(i != 200){
          fprintf(out_file, "%d ", i); // write to file 
	  }
	  else{
		fprintf(out_file, "%d", i); // write to file 
	  }
  }
           
}
