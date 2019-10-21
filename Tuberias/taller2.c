#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#define MAX_BUFF 1024

int main(int argc, char const *argv[])
{

    char buff[MAX_BUFF];
    pid_t padre = getpid();

    int tuberia1[2][2], tuberia2[2][2], tuberia3[2][2], n;

    for (int x = 0; x < 3; x++)
    {
        pipe(tuberia1[x]);
        pipe(tuberia2[x]);
        pipe(tuberia3[x]);
    }

    int i = 0;

    for (; i < 3; i++)
    {
        if (fork() == 0)
        {
            break;
        }
    }


    if (getpid() == padre)
    {
        //Esto es para el promedio
        FILE *archivo;
        archivo = fopen("archivo2.txt", "r");
        int suma;
        // fgets(buff, MAX_BUFF, archivo);
        fgets(buff, MAX_BUFF, archivo);
        write(tuberia1[0][1], buff, sizeof(buff));
        write(tuberia2[0][1], buff, sizeof(buff));
        write(tuberia3[0][1], buff, sizeof(buff));
        while (!feof(archivo))
        {
            fgets(buff, MAX_BUFF, archivo);
            write(tuberia1[0][1], buff, sizeof(buff));
            write(tuberia2[0][1], buff, sizeof(buff));
            write(tuberia3[0][1], buff, sizeof(buff));
        }
        close(tuberia1[0][1]);
        close(tuberia2[0][1]);
        close(tuberia3[0][1]);
        //Cerrar todas las escrituras
        read(tuberia1[1][0], buff, MAX_BUFF);
        printf("El promedio es: %d\n", atoi(buff));
        read(tuberia2[1][0], buff, MAX_BUFF);
        printf("La cantidad de valores impares es: %d\n", atoi(buff));
        read(tuberia3[1][0], buff, MAX_BUFF);
        printf("El numero que mas se repite es: %d\n", atoi(buff));
        
        for(int k=0;k<2;k++)wait(NULL);
        //proceso padre
    }
    else
    {
        if (i == 2)
        {
            //proceso hijo 3
            close(tuberia3[0][1]);
            close(tuberia3[1][0]);
            int vector[MAX_BUFF];
            int n_numeros = 0,z=0, moda = 0, repetidos =0, moda_c=0;
            read(tuberia3[0][0], buff, MAX_BUFF);
            n_numeros = atoi(buff);
            while (z< n_numeros)
            {
                n = read(tuberia3[0][0], buff, MAX_BUFF);
                buff[n] = '\0';
                vector[z] = atoi(buff);
                z++;                
            }
            printf("\n");
            for(int k=0;k<n_numeros;k++){
                repetidos = 0;
                for(int u=0;u<n_numeros;u++){
                    if(u!=k){
                        if(vector[k]==vector[u]){
                            repetidos ++;
                        }
                    }
                    if(moda_c < repetidos){
                        moda = vector[k];
                        moda_c = repetidos;
                    }
                }
            }

            sprintf(buff,"%d",moda);
            write(tuberia3[1][1], buff, sizeof(buff));
            
            close(tuberia3[0][0]);
            //Cerrar el resto de tuberias
            close(tuberia2[0][0]);
            close(tuberia2[0][1]);
            close(tuberia1[0][0]);
            close(tuberia1[0][1]);
            
        }
        if (i == 1)
        {
            //IMPARES
            //proceso hijo 2
            close(tuberia2[0][1]);
            close(tuberia2[1][0]);
            int cantidad = 0, n_numeros = 0, i=0;
            read(tuberia2[0][0], buff, MAX_BUFF);
            n_numeros = atoi(buff);
            while (i< n_numeros)
            {
                n = read(tuberia2[0][0], buff, MAX_BUFF);
                buff[n] = '\0';
                // printf("\n[%d][Hijo 2] Lee: %s \n", getpid(), buff);
                if (atoi(buff) % 2 != 0)
                    cantidad += 1;
                i++;
                
            }
            // printf("\nCantidad de numeros impares: %d\n", cantidad);
            sprintf(buff,"%d",cantidad);
            write(tuberia2[1][1], buff, sizeof(buff));
            close(tuberia2[0][0]);
            //Cerrar resto de tuberias
            close(tuberia1[0][0]);
            close(tuberia1[0][1]);
            close(tuberia3[0][0]);
            close(tuberia3[0][1]);
        }
        if (i == 0)
        {
            //PROMEDIO
            //proceso hijo 1
            close(tuberia1[0][1]);
            close(tuberia1[1][0]);
            int suma = 0, n_numeros = 0;
            //Tuberia 0 cerrado escritura
            //Tuberia 1 cerrado lectura
            read(tuberia1[0][0], buff, MAX_BUFF);
            n_numeros += atoi(buff);
            while ((n = read(tuberia1[0][0], buff, MAX_BUFF)) > 0)
            {
                buff[n] = '\0';
                suma += atoi(buff);
            }
            sprintf(buff,"%d",suma/n_numeros);
            write(tuberia1[1][1], buff, sizeof(buff));
            

            close(tuberia1[0][0]);
            
            //Cerrar el resto de tuberias
            close(tuberia2[0][0]);
            close(tuberia2[0][1]);
            close(tuberia3[0][0]);
            close(tuberia3[0][1]);
        }
    }
}
