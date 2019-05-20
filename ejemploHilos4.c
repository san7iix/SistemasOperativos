#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NTHREADS 6

pthread_barrier_t mybarrier;
void* funcionHilo(void *param);

int main(){
        int i;
        pthread_t hilos[NTHREADS];
        int ids[NTHREADS];

        srand(time(NULL));
        pthread_barrier_init(&mybarrier, NULL,NTHREADS);
        for ( i = 0; i < NTHREADS; ++i)
        {
                ids[i] = i;
                pthread_create(&hilos[i], NULL, funcionHilo,&ids[i]);
        }
        for ( i = 0; i < NTHREADS; ++i)
        {
                pthread_join(hilos[i], NULL);
        }
        pthread_barrier_wait(&mybarrier);
        pthread_barrier_destroy(&mybarrier);
}

void* funcionHilo(void *param){
        int tid = *(int*)param;
        int espera = 1 + rand() %6;

        printf("Hilo %d: bloqueado por %d segundos. \n",tid, espera );
        sleep(espera);
        printf("hilo %d: hecho...\n", tid );

        pthread_barrier_wait(&mybarrier);

        printf("hilo %d: terminando\n", tid);
        return NULL;
}
