#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char const *argv[]){

    pid_t *hijos,padre = getpid(),destinatario;
    int **pd,n,i,flag;
    int size;
    char mensaje[100];
    char s;

    printf("Ingrese la cantidad de hijos: ");
    scanf("%d",&n);
    hijos = (pid_t*) malloc(n*sizeof(pid_t));
    pd = (int**) malloc((n+1)*sizeof(int*));

    for (i=0;i<=n;i++)
        pd[i] = (int *) malloc (2*sizeof(int));

    for(int i=0;i<(n+1);i++)
        pipe(pd[i]);

    for(i=0;i<n;i++){
        hijos[i]=fork();
        if(hijos[i]==0){
            flag=(i+2);
            break;
        }
    }
    if(padre==getpid()){
        printf("Ingrese el mensaje que desea enviar: ");
        scanf("%s", mensaje);
        printf("El mensaje del padre es %s y mi pid es %d\n",mensaje,getpid());

        close(pd[0][0]);
        close(pd[n][1]);
        for(int i=1;i<n;i++){
            close(pd[i][0]);
            close(pd[i][1]);
        }
        int size2=(int)strlen(mensaje);
        int size3;
        int idEnviar = (int) padre;
        write(pd[0][1],&size2,sizeof(int));
        write(pd[0][1],mensaje,size2);
        write(pd[0][1],&idEnviar,sizeof(int));

        read(pd[n][0],&size3,sizeof(int));
        read(pd[n][0],mensaje,size3);
        read(pd[n][0],&destinatario,sizeof(int));
        printf("El mensaje del padre es %s y mi pid es %d y el que lo envio fue %d\n",mensaje,getpid(),destinatario);

    }else{
        close(pd[flag-2][1]);
        close(pd[flag-1][0]);
        for(int i=0;i<(n+1);i++){
            for(int j=0;j<2;j++){
                if(i!=(flag-2)){
                    if(i!=(flag-1)){
                        close(pd[i][j]);
                    }
                }
            }
        }
        int idEnviar = (int) getpid();

        read(pd[flag-2][0],&size,sizeof(int));
        read(pd[flag-2][0],&mensaje,size);
        read(pd[flag-2][0],&destinatario,sizeof(int));
        printf("El mensaje del hijo es %s y mi pid es %d y el que lo envio fue %d\n",mensaje,getpid(),destinatario);
        write(pd[flag-1][1],&size,sizeof(int));
        write(pd[flag-1][1],&mensaje,size);
        write(pd[flag-1][1],&idEnviar,sizeof(int));


    }

    return 0;
}
