#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_BUFF 1024

char *ping = "PING";
char *pong = "PONG";
int i;

int main(int argc, char const *argv[]) {

  int tuberia1[2],tuberia2[2],n;
  char buff[MAX_BUFF];
  pipe(tuberia1);
  pipe(tuberia2);

  for (i = 0; i < 2; i++) {
    if(fork()==0){
      break;
    }
  }



  if(i==2){
    close(tuberia1[0]);
    close(tuberia1[1]);
    close(tuberia2[0]);
    close(tuberia2[1]);
    printf("[%i]Padre\n", getpid() );
    wait(NULL);

  }else if (i==1) {
    close(tuberia1[1]);
    close(tuberia2[0]);

      while(n=read(tuberia1[0],buff,MAX_BUFF)>0){
        printf("[%d]%s\n",getpid(),buff);
      }
      close(tuberia1[0]);
      close(tuberia2[1]);

  } else {
      close(tuberia1[0]);
      close(tuberia2[1]);
      strcpy(buff,ping);

      for (int k = 0; k < 10; k++) {
        printf("[%d]Ping\n",getpid());
        write(tuberia1[1],buff,sizeof(buff));
      }
      close(tuberia1[1]);
      close(tuberia2[0]);
  }



  return 0;
}
