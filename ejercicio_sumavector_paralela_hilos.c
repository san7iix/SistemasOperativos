#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 5

void* funcion_maneja_hilo(void *);
void llenarVector(int *v);
void imprimirVector(int *v);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion;
int suma_global=0;
typedef unsigned long int tipo_hilo;
struct info {
        int numero_proceso;
        int tope_min;
        int tope_max;
};

int finalizar=0,t_vector,n_hilos;

int suma,*v;

int main(int argc, char const *argv[]) {
        printf("Cantidad de hilos\n");
        scanf("%d",&n_hilos);
        printf("Longitud del vector\n");
        scanf("%d",&t_vector);
        v=(int *)malloc(sizeof(int)*t_vector);
        pthread_t pid_hilo[n_hilos];
        struct info *Info;
        int i,division=t_vector/n_hilos;
        llenarVector(v);
        for (i = 0; i < n_hilos; i++) {
                Info = (struct info *)malloc(sizeof(struct info));
                Info->numero_proceso = i;
                Info->tope_min=i*division;
                Info->tope_max=Info->tope_min+division;
                pthread_create(&pid_hilo[i],NULL,funcion_maneja_hilo,(void*) Info);
        }
        printf("Hilo principal (idthread ->[%lu])\n",pthread_self());
        for ( i = 0; i < n_hilos; i++) {
                pthread_join(pid_hilo[i],NULL);
        }
        pthread_cond_init(&condicion,NULL);
        while(finalizar!=n_hilos)pthread_cond_wait(&condicion,&mutex);
        printf("Suma completa [%d]\n",suma_global);

        return 0;
}

void* funcion_maneja_hilo(void *param) {
        int suma=0;
        struct info *infoLlegada = ((struct info *)param);
        for (size_t i = infoLlegada->tope_min; i < infoLlegada->tope_max; i++) {
                suma=suma+v[i];
        }
        printf("Hilo nº-> %d : Topemin: %d, :Topemax %d, Suma: [%d]\n", infoLlegada->numero_proceso+1, infoLlegada->tope_min, infoLlegada->tope_max,suma);
        if(infoLlegada->numero_proceso==(n_hilos-1) && t_vector%n_hilos!=0){
          for (size_t i = t_vector-1; i >=infoLlegada->tope_max; i--) {
                  suma=suma+v[i];
          }
        }
        pthread_mutex_lock(&mutex);
        suma_global+=suma;
        finalizar+=1;
        pthread_mutex_unlock(&mutex);
        pthread_exit(0);
}

void llenarVector(int *v){
        FILE *file;
        char cadena[15];
        int numero,i=0;
        file = fopen("archivo.txt","r");
        fgets(cadena,15,file);
        while (!feof(file) && i<t_vector) {
                numero = atoi(cadena);
                v[i]=numero;
                i++;
                fgets(cadena,15,file);
        }
}

void imprimirVector(int *v){
        for (size_t i = 0; i < t_vector; i++) {
                printf("%i\n",v[i] );
        }
}
