#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define size 4

void manejador(int s){
};

int main(){

        pid_t hijos[size], parent;
        int i;
        signal(SIGUSR1, manejador);
        parent = getpid();
        for(i=0; i<2; ++i) {
                if(!(hijos[i]=fork())) {//si es hijo del primer proceso
                        hijos[i+1]=fork();
                        break;
                }
        }
/*
    if(parent==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        for(int j = 0;j < size+size;j++){
            wait(NULL);
            //printf("hijo[%d] retorno %d\n",j,WEXITSTATUS(status));
        }
        printf("Padre finalizado\n");
    }else{
        sleep(1);
    }
 */
        //Aqui incia el ciclo(luego de creados los procesos)
        if(parent == getpid()) {
                printf("Proceso padre [pid=%d]\n",getpid());
                // usleep(100);
                kill(hijos[--i], SIGUSR1);
                pause();
                printf("Proceso padre [pid=%d]\n",getpid());
                for(i=0; i<size; ++i) {
                        wait(NULL);
                }
        }
        else{
                // pause();
                printf("hijo %d Proceso [pid=%d]\n",(i+1),getpid());//i+1 para que no imprima hijo 0..
                if(i==0) {//si es el primer hijo
                        //pause();
                        //este kill va luego de volver a quitarle el pause, cuando va hacia arriba nuevamente
                        // usleep(100);
                        kill(hijos[i+1], SIGUSR1);

                }else{
                        // usleep(100);
                        kill(hijos[i-1], SIGUSR1);//como el 2 no tiene hijos no hay problema
                }
        }
        return 0;
}
