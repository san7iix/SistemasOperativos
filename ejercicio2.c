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

  //Lectura 0 escritura 1
  if(i==2){
    close(tuberia1[1]);
    close(tuberia2[1]);
    wait(NULL);
    printf("-----------Archivo1---------------\n");
    while((n=read(tuberia2[0],buff,MAX_BUFF))>0){
      buff[n] = '\0';
      printf("[Padre][%d] Lee: %s",getpid(),buff);
    }

    printf("-----------Archivo2---------------\n");

    while((n=read(tuberia1[0],buff,MAX_BUFF))>0){
      buff[n] = '\0';
      printf("[Padre][%d] Lee: %s",getpid(),buff);
    }
    close(tuberia2[0]);
    close(tuberia1[0]);
  }else if(i==1){
    close(tuberia2[0]);
    close(tuberia1[1]);
    close(tuberia1[0]);
    printf("Proceso 2 envia por tuberia 2\n");
    FILE *archivo;
    archivo = fopen("archivo.txt","r");

    fgets(buff,MAX_BUFF,archivo);
    while (!feof(archivo)){
     write(tuberia2[1],buff,sizeof(buff));
     fgets(buff,MAX_BUFF,archivo);
    }
    close(tuberia2[0]);

  }else{
    close(tuberia1[0]);
    close(tuberia2[1]);
    close(tuberia2[0]);

    printf("Proceso 1 envia por tuberia 1\n");
    FILE *archivo;
    archivo = fopen("archivo2.txt","r");

    fgets(buff,MAX_BUFF,archivo);
    while (!feof(archivo)){
     write(tuberia1[1],buff,sizeof(buff));
     fgets(buff,MAX_BUFF,archivo);
    }
    close(tuberia1[0]);
  }

}
