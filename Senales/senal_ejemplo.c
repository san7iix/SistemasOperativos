#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

//Declaramos los prototipos de funciones
void manejador(int signum);

//Variable global
int bandera = 1;

//Función principal
int main(int argc, char **argv)
{
        //Declaramos variables
        int status,pid;

        //Si fork() es igual a 0 entonces es hijo
        if((pid=fork())==0)
        {
                //Proceso hijo
                printf("Soy hijo y estoy esperando una señal de mi padre, mi pid es: %d\n",getpid());
                //Capturamos la señal SIGUSR1
                signal(SIGUSR1,manejador);
                //Utilizamos la variable bandera para que el proceso no termine
                while(bandera);
                //Usamos el comando kill para enviar una señal
                //Se necesita el pid del padre y la señal
                kill(getppid(),SIGUSR2);
        }
        else
        {
                //Proceso padre
                //Capturamos la señal SIGUSR2
                signal(SIGUSR2,manejador);
                printf("Soy Padre, mi pid es: %d\n",getpid());
                //Esperamos 3 segundos
                sleep(3);
                //Usamos el comando kill para enviar una señal
                //Se necesita el pid y la señal
                kill(pid, SIGUSR1);
                //Esperamos que termine el hijo
                wait(&status);
                printf("Mi hijo termino con un estado: %d\n",status);
        }
}

//Funcion manejador
void manejador (int signum){
        //Si la señal es SIGUSR1 entonces
        if(signum==SIGUSR1) {
                printf("Recibi una senal de mi padre %d\n",signum);
        }
        //Si es SIGUSR2 entonces
        else
        {
                printf("Recibi una senal de mi hijo %d\n",signum);
        }
        bandera=0;
}
