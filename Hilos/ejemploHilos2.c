
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<wait.h>

#define size 3

void manejador(int s){};

int main(){

    pid_t hijos[size], parent;
    int i;
    signal(SIGUSR1, manejador);
    parent = getpid();
    for(i=0;i<2;i++){
        if(!(hijos[i]=fork())){//si es hijo del primer proceso
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
    if(parent==getpid()){
        printf("Soy el padre, pid=[%d]\n", getpid());
        sleep(1);
        kill(hijos[1], SIGUSR1);
        pause();
        printf("Soy el padre, pid=[%d]\n", getpid());
    }else{
        if(parent==getppid()){
            if (i==0) {
                pause();
                printf("Hijo [%d], pid=%d\n", i+1,getpid());
                kill(hijos[1],SIGUSR1);
                pause();
                printf("Hijo [%d], pid=%d\n", i+1,getpid());
                kill(getppid(), SIGUSR1);
                
            }else{
                pause();
                printf("Hijo [%d], pid=%d\n", i+1,getpid());
                kill(hijos[2],SIGUSR1);
                pause();
                printf("Hijo [%d], pid=%d\n", i+1,getpid());
                kill(hijos[0], SIGUSR1);
            }   
        }else{
            if (i==0) {
                pause();
                printf("Hijo [%d,1], pid=%d\n", i+1,getpid());
                kill(getppid(),SIGUSR1);  
            }else{
                pause();
                printf("Hijo [%d,2], pid=%d\n", i+1,getpid());
                kill(getppid(),SIGUSR1); 
            }
        }
    }
    return 0;
}