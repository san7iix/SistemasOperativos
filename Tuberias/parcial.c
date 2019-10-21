#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_BUFF 1024

int main(int argc, char const *argv[]) {


  char buff[MAX_BUFF];
  int tuberia1[12][2];
  int i,n,k=0;
  int padre = getpid();

  for (int j = 0; j < 12; j++) {
    pipe(tuberia1[j]);
  }

  for (i = 0; i < 2; i++) {
    if(!fork()){
      if(!fork()){
        if(!fork()){
          break;
        }
        break;
      }
      break;
    }
  }

  if(getpid()==padre){
    for (size_t k = 0; k < 12; k++) {
      if(k==0 || k==7){
        close(tuberia1[k][0]);
      }else if(k==5 || k==11){
        close(tuberia1[k][1]);
      }else{
        close(tuberia1[k][0]);
        close(tuberia1[k][1]);
      }
    }
    scanf("%s",buff);
    write(tuberia1[0][1],buff,sizeof(buff));
    close(tuberia1[0][1]);
    wait(NULL);
    printf("[%d] PADRE \n",getpid());
  }else{
    printf("%d\n",getpid() );
  }

  return 0;
}
