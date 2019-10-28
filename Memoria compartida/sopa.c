#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_BUFF 1024


unsigned int sizeof_dm(int rows, int cols, size_t sizeElement);
void create_index(void **m, int rows, int cols, size_t sizeElement);
void llenarMatriz(int nRows, int nCols, char** matrix);
void printMatrix(int rows, int cols, char **m);
void buscarPalabrasHorizontal(int nRows, int nCols, char** matrix);
void buscarPalabrasVertical(int nRows, int nCols, char** matrix);

typedef struct palabras palabras;
struct palabras{
  char *palabras;
  palabras *sig;
};


int main(int argc, char const *argv[]) {


    int nRows=6,nCols=6,shmIdA;
    char** matrixA;

    size_t sizeMatrixA = sizeof_dm(nRows,nCols,sizeof(char));
    shmIdA = shmget(IPC_PRIVATE, sizeMatrixA, IPC_CREAT|0600);
    matrixA = shmat(shmIdA, NULL, 0);
    create_index((void*)matrixA, nRows, nCols, sizeof(char));
    llenarMatriz(nRows,nCols,matrixA);
    printMatrix(nRows,nCols,matrixA);
    buscarPalabrasVertical(nRows,nCols,matrixA);

    
}

void printMatrix(int rows, int cols, char **m){
    for (int x = 0; x < rows; ++x)
        {
            for (int y = 0; y < cols; ++y)
            {
                printf("[%c]", m[x][y]);
            }
            printf("\n");
        }
}


unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
  size_t size = rows * sizeof(void *);
  size += (cols * rows * sizeElement);
  return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
  int i;
  size_t sizeRow = cols * sizeElement;
  m[0] = m + rows;
  for (i = 1; i < rows; i++) {
    m[i] = (m[i-1] + sizeRow);
  }
}

void llenarMatriz(int nRows, int nCols, char** matrix){

    FILE *archivo;
    char buff[MAX_BUFF];
    size_t tam;
    int i =0;
    archivo = fopen("sopa.txt","r");
    fgets(buff,MAX_BUFF,archivo);
    fgets(buff,MAX_BUFF,archivo);

    while (!feof(archivo)){
        fgets(buff, MAX_BUFF, archivo);
        sprintf(matrix[i],"%s",buff);
        i++;
    }
    

}

void buscarPalabrasHorizontal(int nRows, int nCols, char** matrix){
    char buff[MAX_BUFF];
    char * palabra = "PERRO";
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            buff[j] = matrix[i][j];
        }
        if(strstr(buff,palabra)!=NULL){
            printf("\nSe encontro: %s en la fila %d \n", palabra, i );
        }
    }
    
}

void buscarPalabrasVertical(int nRows, int nCols, char** matrix){
    char buff[MAX_BUFF];
    char * palabra = "SES";
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            buff[j] = matrix[j][i];
        }
        if(strstr(buff,palabra)!=NULL){
            printf("\nSe encontro: %s en la columna %d \n", palabra, i );
        }
    }
    
}

