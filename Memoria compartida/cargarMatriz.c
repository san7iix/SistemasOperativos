#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned int sizeof_dm(int rows, int col, size_t sizeElement){

    size_t size = rows * sizeof(double *);
    size += (col*rows*sizeElement);
    return size;

}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i=0;
    size_t sizeRow = cols * sizeElement;
    m[0] = m+rows;
    for (i = 1; i < cols; ++i)
    {
        m[i] = (m[i-1] + sizeRow);
    }
}


void llenarMatrix(int rows, int cols, double **m){
    FILE *Archivo;
    int filas, columnas;
    int elemento;
    Archivo = fopen ("Datos.txt", "r");
    if(Archivo!=NULL){
        fscanf(Archivo, "%d", &filas);
		fscanf(Archivo, "%d", &columnas);
        while(!feof(Archivo)){
            for(int i=0; i<rows; i++){
				for(int j=0; j<cols; j++){
					fscanf(Archivo, "%d", &elemento);
                    m[i][j]=elemento;
				}

			}

        }
    }
}

void printMatrix(int rows, int cols, double **m){
    for (int x = 0; x < rows; ++x)
        {
            for (int y = 0; y < cols; ++y)
            {
                printf("[%.1f]\t", m[x][y]);
            }
            printf("\n");
        }
}

double filtro(int fil, int col, double **m, int n){
    double promedio = 0.0, suma = 0.0;
    int iniFil = 0, finFil = 0, iniCol = 0, finCol = 0;
    if ((fil!=0 && fil!=n)&&(col!=0 && col!=n))
    {
            iniFil = fil-1;
            finFil = fil+1;

            iniCol = col-1;
            finCol = col+1;

            for (int i = iniFil; i <= finFil; ++i)
            {
                for (int j = iniCol; j <= finCol; ++j)
                {
                    suma += m[i][j];
                    //printf("%.1f\t", m[i][j]);
                }
            }
            promedio = suma/9;
    }else{
        promedio = m[fil][col];
    }
    return promedio;
}

int main(int argc, char const *argv[])
{


    int filas, columnas,n_hijos, trabajo;
    FILE *Archivo;
    Archivo = fopen ("Datos.txt", "r");
    if(Archivo != NULL){
        fscanf(Archivo, "%d", &filas);
        fscanf(Archivo, "%d", &columnas);
    }
    fclose(Archivo);
    

    printf("Ingrese el número de hijos\n");
    scanf("%d",&n_hijos);
    trabajo = filas / n_hijos;
    int padre = getpid();
    pid_t hijos;

    int shm_idI;
    int shm_idII;

    size_t sizeMatrix;

    double** matrix;
    double** matrixFil;

    int l =0;

    sizeMatrix = sizeof_dm(filas,columnas,sizeof(double));
    shm_idI =  shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0666);
    shm_idII = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0666);

    matrix = shmat(shm_idI, NULL, 0);
    matrixFil = shmat(shm_idII, NULL, 0);

    create_index((void*)matrix, filas, columnas, sizeof(double));
    create_index((void*)matrixFil, filas, columnas, sizeof(double));
    llenarMatrix( filas, columnas, matrix);


    // pid_t hijo = fork();
    int i=0;
    for(;i<n_hijos;i++){
        if(fork())break;
    }

    if (i==n_hijos) // padre
    { 
        // for(int esperar=0;esperar<n_hijos;n_hijos)wait(NULL);
        printf("\nMatriz inicial\n");
        printMatrix( filas, columnas, matrix);
        sleep(1);
        printf("\nMatriz final\n");
        printMatrix(filas, columnas, matrixFil);
    }else{//hijo
        int n=i+1;
        for (int y = i*trabajo; y <trabajo*n; y++)
        {
            for (int x = 0; x < columnas; x++)
            {
                matrixFil[y][x] = filtro(y, x, matrix, columnas-1);
            }
        }
        printf("\n");
    }

    return 0;
}
