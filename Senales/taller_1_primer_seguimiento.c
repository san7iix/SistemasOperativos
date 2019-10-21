#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>

int n_veces, turno=0,i;
void handler(){};

int main(int argc, char const *argv[]) {
  signal(SIGUSR1, handler);
  int pids[2];
  // printf("Ingrese la cantidad de veces a ejecutar\n");
  // scanf("%d",&n_veces);
  for (i = 0; i < 2; i++) {
    pids[i] = fork();
    if(pids[i]==0){
      break;
    }
  }
  if(i==0){
    // Hijo 1
    if(fork()==0){
      printf("[%d] Hijo 1-2\n", getpid());
    }else{
      pause();
      printf("[%d] Hijo 1\n", getpid());
    }
  }
  if(i==1){
    // Hijo2
    if(fork()==0){
      printf("[%d] Hijo 2-2\n", getpid());
    }else{
      printf("[%d] Hijo 2 %d\n", getpid(),turno);
    }
  }
  if(i==2){
    printf("[%d] Padre\n", getpid() );
    kill(pids[0],SIGUSR1);
  }
  exit(0);
  return 0;
}
