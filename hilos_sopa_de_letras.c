#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 5
#define MAX_N 50

void* funcion_maneja_hilo1(void *);
void* funcion_maneja_hilo2(void *);
void llenarMatriz();
void imprimirMatriz();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int filas,columnas,palabras_a_buscar;
struct palabras {
  char *palabra1;
  char *palabra2;
  char *palabra3;
};

int matriz_sopa[MAX_N][MAX_N];
char matriz_palabras[MAX_N][MAX_N];

int main(int argc, char const *argv[]) {
  llenarMatriz();
  imprimirMatriz();
  pthread_t pid_hilo[2];
  struct palabras *pal = (struct palabras *)malloc(sizeof(struct palabras));
  pal->palabra1="EXAMEN";
  pal->palabra2="GANAR";
  pal->palabra3="SUERTE";
  pthread_create(&pid_hilo[0],NULL,funcion_maneja_hilo1,(void*) pal);
  pthread_create(&pid_hilo[1],NULL,funcion_maneja_hilo2,(void*) pal);
  return 0;
}

void llenarMatriz(){
        FILE *file;
        char cadena[15];
        file = fopen("archivo2.txt","r");
        fgets(cadena,15,file);
        filas = atoi(cadena);
        fgets(cadena,15,file);
        columnas = atoi(cadena);
        fgets(cadena,15,file);
        int j=0;
        while (j<filas) {
          for (size_t i = j; i < filas; i++) {
            for (size_t k = 0; k < columnas; k++) {
              matriz_sopa[i][k]=cadena[k];
            }
          }
          fgets(cadena,15,file);
          j++;
        }
        fgets(cadena,15,file);
        palabras_a_buscar = atoi(cadena);
        fgets(cadena,15,file);
        for (size_t z = 0; z < palabras_a_buscar && !feof(file); z++) {

        }
        fclose(file);
}

void imprimirMatriz(){
  for (size_t i = 0; i < filas; i++) {
    for (size_t k = 0; k < columnas; k++) {
      printf("[%c]",matriz_sopa[i][k]);
    }
    printf("\n");
  }
}

void* funcion_maneja_hilo1(void *param){
  
}

void* funcion_maneja_hilo2(void *param){

}
