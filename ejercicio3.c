#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_BUFF 1024
int i,contadorVocales=0,indice;
char buff[MAX_BUFF],letra[1];

int main(int argc, char const *argv[]) {

  int tuberia1[2],tuberia2[2],n;
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
    close(tuberia2[0]);
    wait(NULL);
    while((n=read(tuberia1[0],buff,MAX_BUFF))>0){
      buff[n] = '\0';
    }
    getchar();

  }else if(i==1){
    close(tuberia1[1]);
    close(tuberia1[0]);
    close(tuberia2[0]);
    FILE *archivo;
    archivo = fopen("archivo.txt","r");
    fgets(buff,MAX_BUFF,archivo);
    while (!feof(archivo)) {
      write(tuberia2[1],buff,sizeof(buff));
      fgets(buff,MAX_BUFF,archivo);
    }
    printf("[%d]Archivo enviado por tuberia\n",getpid());
    fclose(archivo);
    close(tuberia2[1]);
  }else{
    close(tuberia1[0]);
    close(tuberia2[1]);
    while((n=read(tuberia2[0],buff,MAX_BUFF))>0){
      buff[n] = '\0';
      contadorVocales=0;
      for (int k = 0; k < strlen(buff); k++) {
        if(buff[k]=='a' || buff[k]=='e' || buff[k]=='i' || buff[k]=='o' || buff[k]=='u')contadorVocales++;
      }
      write(tuberia1[1],buff,sizeof(buff));
      printf("[Hijo 1][%d]Numero de vocales: %i\n",getpid(),contadorVocales);
    }

    close(tuberia1[1]);
    close(tuberia2[0]);
  }

  return 0;
}
