#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 5

void* funcion_maneja_hilo(void *);
typedef unsigned long int tipo_hilo;

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
  printf("Hilo principal (idthread ->[%lu])\n",pthread_self());
  for ( i = 0; i < N; i++) {
    pthread_join(pid_hilo[i],NULL);
  }
  return 0;
}

void* funcion_maneja_hilo(void *param) {
  printf("Hilo param -> %d (idthread -> [%lu])\n",((struct nodo *)param)->val, pthread_self());
  free(param);
  pthread_exit(0);
}
