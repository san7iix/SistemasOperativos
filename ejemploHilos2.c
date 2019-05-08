#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 5

void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;
int turno=-1;

struct nodo {
  int val;
};

int main(int argc, char const *argv[]) {
  pthread_t pid_hilo[N];
  struct nodo *Nodo;
  int i;
  for (i = 0; i < N; i++) {
    Nodo = (struct nodo *)malloc(sizeof(struct nodo));
    Nodo->val = i;
    pthread_create(&pid_hilo[i],NULL,funcion_maneja_hilo,(void*) Nodo);
  }
  turno+=1;
  printf("Hilo principal (idthread ->[%lu])\n",pthread_self());
  for ( i = 0; i < N; i++) {
    pthread_join(pid_hilo[i],NULL);
  }
  while (turno!=(N));
  printf("Hilo principal (idthread ->[%lu])\n",pthread_self());
  return 0;
}

void* funcion_maneja_hilo(void *param) {
  while(turno!= ((struct nodo *)param)->val);
  printf("Hilo param -> %d (idthread -> [%lu])\n",((struct nodo *)param)->val, pthread_self());
  free(param);
  turno+=1;
  pthread_exit(0);
}
