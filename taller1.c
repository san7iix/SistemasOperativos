#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_BUFF 1024

int main(int argc, char const *argv[]) {

  char buff[MAX_BUFF];
  int nProcesos,i,n;
  printf("Cantidad de hijos: \n");
  scanf("%d",&nProcesos);
  int tuberia1[nProcesos+1][2];

  for (int j = 0; j < nProcesos+1; j++) {
    pipe(tuberia1[j]);
  }

  for (i = 0; i < nProcesos; i++) {
    if(fork()==0){
      break;
    }
  }

  if(i==nProcesos){
    printf("Ingrese la cadena a comunicar\n");
    scanf("%s",buff);
    close(tuberia1[0][0]);
    close(tuberia1[nProcesos][1]);
    for (int k = 1; k < nProcesos; k++) {
      close(tuberia1[k][0]);
      close(tuberia1[k][1]);
    }
    write(tuberia1[0][1],buff,sizeof(buff));
    close(tuberia1[0][1]);
    wait(NULL);
    read(tuberia1[nProcesos][0],buff,MAX_BUFF);
    printf("[%d]Proceso padre, imprime: %s\n",getpid(), buff);
  }else{
    close(tuberia1[i][1]);
    close(tuberia1[i+1][0]);
    for (int k = 0; k < nProcesos+1; k++) {
      if(k!=i && k!=(i+1)){
        close(tuberia1[k][0]);
        close(tuberia1[k][1]);
      }
    }
    read(tuberia1[i][0],buff,MAX_BUFF);
    write(tuberia1[i+1][1],buff,sizeof(buff));
    printf("[%d]Proceso hijo %d, imprime: %s\n",getpid(),i, buff);
  }
  return 0;
}
