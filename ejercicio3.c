#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_BUFF 1024
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
    close(tuberia2[0]);
    wait(NULL);

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
    printf("[%d]Archivo enviado por tuberia\n",getpid() );
    fclose(archivo);
    close(tuberia2[1]);
  }else{

  }

}
