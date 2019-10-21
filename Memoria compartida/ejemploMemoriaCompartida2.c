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
    int i=0, j=0, count = 0;
    for (i = 0; i < rows; ++i)
    {
        for (j = 0; j < cols; ++j)
        {
            m[i][j] = ++count;
        }
    }
}

void printMatrix(int rows, int cols, double **m){
    for (int x = 0; x < rows; ++x)
        {
            for (int y = 0; y < cols; ++y)
            {
                printf("%.1f\t", m[x][y]);
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



    int n=0, nhijos=0;
    printf("INGRESE EL NUMERO DE COLUMNAS Y FILAS: ");
    scanf("%d", &n);
    //printf("INGRESE EL NUMERO DE HIJOS: ");
    //scanf("%d", &nhijos);
    int padre = getpid();
    pid_t hijos;

    int shm_idI;
    int shm_idII;

    size_t sizeMatrix;

    double** matrix;
    double** matrixFil;

    int l =0;

    sizeMatrix = sizeof_dm(n,n,sizeof(double));
    shm_idI =  shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0666);
    shm_idII = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0666);

    matrix = shmat(shm_idI, NULL, 0);
    matrixFil = shmat(shm_idII, NULL, 0);

    create_index((void*)matrix, n, n, sizeof(double));
    create_index((void*)matrixFil, n, n, sizeof(double));


    pid_t hijo = fork();

    if (hijo != 0) // padre
    {
        llenarMatrix( n, n, matrix);

        wait(NULL);
    }else{//hijo
        usleep(1000);
        printMatrix( n, n, matrix);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrixFil[i][j] = filtro(i, j, matrix, n-1);
                //printf("\n");
            }
        }
        printf("\n");
        printMatrix(n, n, matrixFil);
    }

    return 0;
}
