#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement);
void create_index(void **m, int rows, int cols, size_t sizeElement);
void llenarMatriz(int nRows, int nCols, double** matrix);
void imprimirMatriz(int rows, int cols, double **m);
void modificarMatriz(int row, int col, double **matrix);

int main(int argc, char const *argv[]) {
  int nRows=5,nCols=10,shmId;
  double** matrix;

  size_t sizeMatrix = sizeof_dm(nRows,nCols,sizeof(double));
  shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
  matrix = shmat(shmId, NULL, 0);
  create_index((void*)matrix, nRows, nCols, sizeof(double));

  if(fork()){
    printf("[%d]Hijo\n",getpid() );
    imprimirMatriz(nRows,nCols,matrix);
    modificarMatriz(2,2,matrix);
  }else{
    llenarMatriz(nRows,nCols,matrix);
    printf("[%d]Padre\n",getpid() );
    wait(NULL);
    imprimirMatriz(nRows,nCols,matrix);
  }
  return 0;
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

void llenarMatriz(int nRows, int nCols, double** matrix){
  double incr=0;
  for (int i = 0; i < nRows; i++) {
    for (int k = 0; k < nCols; k++) {
      matrix[i][k] = incr++;
    }
  }
}

void imprimirMatriz(int rows, int cols, double **matrix){
    for (int x = 0; x < rows; ++x){
            for (int y = 0; y < cols; ++y){
                printf("%.2f\t", matrix[x][y]);
            }
            printf("\n");
        }
}

void modificarMatriz(int row, int col, double **matrix){
  matrix[row][col] = (-99.99);
}
