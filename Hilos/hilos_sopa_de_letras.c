#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 5
#define MAX_N 50

void* funcion_maneja_hilo1(void *);
void* funcion_maneja_hilo2(void *);
void llenarMatriz();
void imprimirMatriz();
void imprimirMatrizPalabras();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int filas,columnas,palabras_a_buscar;
char **palabras;
int n_palabras,t_palabras;

int matriz_sopa[MAX_N][MAX_N];
char matriz_palabras[MAX_N][MAX_N];

int main(int argc, char const *argv[]) {
  llenarMatriz();
  // imprimirMatriz();
  pthread_t pid_hilo[2];
  pthread_create(&pid_hilo[0],NULL,funcion_maneja_hilo1,NULL);
  // pthread_create(&pid_hilo[1],NULL,funcion_maneja_hilo2,NULL);
  for (size_t u = 0; u < 1; u++) {
    pthread_join(pid_hilo[u],NULL);
  }
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
        palabras_a_buscar = atoi(cadena);
        fgets(cadena,15,file);
        for (size_t z = 0; z < palabras_a_buscar && !feof(file); z++) {
          for (size_t i = 0; i < strlen(cadena); i++) {
            matriz_palabras[z][i] = cadena[i];
          }
          fgets(cadena,15,file);
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

void imprimirMatrizPalabras(){
  for (size_t i = 0; i < palabras_a_buscar; i++) {
    for (size_t k = 0; k < strlen(matriz_palabras[i]); k++) {
      printf("[%c]",matriz_palabras[i][k]);
    }
    printf("\n");
  }
}

void* funcion_maneja_hilo1(void *param){
        char *palabra,*sopa;
        int q=0;
        while(q < palabras_a_buscar){
                printf("%s\n",matriz_sopa[q]);
                // for (int i = 0; i < filas; i++) {
                //   if(strstr(sopa,palabra)){
                //     printf("Se encontro la palabra %s\n",palabra);
                //     break;
                //   }
                // }
                q++;
        }

}

void* funcion_maneja_hilo2(void *param){

}
