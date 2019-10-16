#include <stdio.h>

int main()

{

	int Matriz[10][10];
	int X;
	int Y;
	int ptrX;
	int ptrY;
	FILE *Archivo;
    Archivo = fopen ("Datos.txt", "r");
	if(Archivo != NULL)
	{
		while (feof(Archivo) == 0)
        {
			fscanf(Archivo, "%d", &Y);
			fscanf(Archivo, "%d", &X);
			printf("%d, %d \n", X, Y);
			for(ptrY=0; ptrY<Y; ptrY++)	
			{
				for(ptrX=0; ptrX<X; ptrX++)
				{
					fscanf(Archivo, "%d", &Matriz[ptrX][ptrY]);
					printf("%d ", Matriz[ptrX][ptrY]);
				}
				printf("\n");
			}
        }
	}
    fclose ( Archivo );
    return 0;

}