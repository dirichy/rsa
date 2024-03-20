#include <stdio.h>
int main(){
  FILE*file=fopen("test.txt","w");
  if (file != NULL){
    fprintf(file,"BYL is sb!");
    fclose(file);
  }
  else{
    perror("Error opening file");
  }
  return 0;
}
