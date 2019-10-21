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
void multiplicarMatrices(int row,int col, double **matrixA, double **matrixB, double **matrixC);
void multiplicarAnillo(int row,int col, double **matrixA, double **matrixB, double **matrixC,int i);

int main(int argc, char const *argv[]) {
  int nRows,nCols,shmIdA,shmIdB,shmIdC,i;
  int RowsB,ColsB;
  int RowsC,ColsC;
  double** matrixA,**matrixB,**matrixC;

  printf("Numero de filas matriz A\n");
  scanf("%d",&nRows);
  printf("Numero de columnas matriz A\n");
  scanf("%d",&nCols);

  RowsB=nCols;
  ColsB=nRows;

  size_t sizeMatrixA = sizeof_dm(nRows,nCols,sizeof(double));
  size_t sizeMatrixB = sizeof_dm(RowsB,ColsB,sizeof(double));
  size_t sizeMatrixC = sizeof_dm(nRows,nRows,sizeof(double));
  shmIdA = shmget(IPC_PRIVATE, sizeMatrixA, IPC_CREAT|0600);
  shmIdB = shmget(IPC_PRIVATE, sizeMatrixB, IPC_CREAT|0600);
  shmIdC = shmget(IPC_PRIVATE, sizeMatrixC, IPC_CREAT|0600);
  matrixA = shmat(shmIdA, NULL, 0);
  matrixB = shmat(shmIdB, NULL, 0);
  matrixC = shmat(shmIdC, NULL, 0);
  create_index((void*)matrixA, nRows, nCols, sizeof(double));
  create_index((void*)matrixB, nCols, nRows, sizeof(double));
  create_index((void*)matrixC, nRows, nRows, sizeof(double));
  llenarMatriz(nRows,nCols,matrixA);
  llenarMatriz(nCols,nRows,matrixB);

  for (i = 0; i < (nRows/2); i++) {
    if(fork()==0){
      break;
    }
  }

  if(i==(nRows/2)){
    for (size_t l = 0; l < (nRows/2); l++) {
      wait(NULL);
    }
    imprimirMatriz(nRows,nCols,matrixC);

  }else{
    multiplicarAnillo(nRows,nCols,matrixA,matrixB,matrixC,i);
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

void multiplicarMatrices(int row,int col, double **matrixA, double **matrixB, double **matrixC){
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      matrixC[i][j]=0;
      for (int k = 0; k < row; k++) {
        matrixC[i][j] = (matrixA[i][k]*matrixB[k][j])+ matrixC[i][j];
      }
    }
  }
}

void multiplicarAnillo(int row,int col, double **matrixA, double **matrixB, double **matrixC, int i){
  for (int x=i; x < (row-i); x++) {
    for (int j = i; j < (col-i); j++) {
      matrixC[x][j]=0;
      if(x==i || x==(row-i-1) || j==i || j==(col-i-1)){
        for (int k = 0; k < row; k++) {
          matrixC[x][j] = (matrixA[x][k]*matrixB[k][j])+ matrixC[x][j];
        }
         printf("[%d]Hijo, posicion[%d,%d]: %.2f\n",getpid(),x,j, matrixC[x][j]);
      }
    }
  }
  printf("\n");
}
