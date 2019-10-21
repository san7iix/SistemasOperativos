#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#define MAX_BUFF 1024

int main()
{
  int fd[2][2], n = 1;
  char buff[MAX_BUFF];
  int i;
  for (int j = 0; j < 12; j++)
  {
    pipe(fd[j]);
  }

  for (i = 0; i < 2; i++)
  {
    if (fork() == 0)
    {
      break;
    }
  }
  if (i == 2)
  {
    int turno = 1;
    int leer = 1;
    close(fd[0][1]);
    close(fd[1][1]);
    while (n > 0)
    {
      if(n<0)break;
      if (turno == 0)
      {
        n = read(fd[1][0], buff, MAX_BUFF);
        buff[n] = '\0';
        printf("\n%d Del [Hijo 2] Lee: %s", getpid(), buff);
        turno = 1;
      }
      else if (turno == 1)
      {
        n = read(fd[0][0], buff, MAX_BUFF);
        buff[n] = '\0';
        printf("\n%d Del [Hijo 1] Lee: %s", getpid(), buff);
        turno = 0;
      }
    }
    close(fd[0][0]);
    close(fd[1][0]);
    for (int w = 0; w < 2; w++)
      wait(NULL);
  }
  else if (i == 0)
  {
    close(fd[0][0]);
    close(fd[1][0]);
    close(fd[1][1]);
    FILE *archivo;
    archivo = fopen("archivo2.txt", "r");
    fgets(buff, MAX_BUFF, archivo);
    while (!feof(archivo))
    {
      write(fd[0][1], buff, sizeof(buff));
      fgets(buff, MAX_BUFF, archivo);
    }
    write(fd[0][1], buff, sizeof(buff));
    fclose(archivo);
    close(fd[0][1]);
  }
  else if (i == 1)
  {
    close(fd[0][0]);
    close(fd[0][1]);
    close(fd[1][0]);
    FILE *archivo;
    archivo = fopen("archivo.txt", "r");
    fgets(buff, MAX_BUFF, archivo);
    while (!feof(archivo))
    {
      write(fd[1][1], buff, sizeof(buff));
      fgets(buff, MAX_BUFF, archivo);
    }
    write(fd[1][1], buff, sizeof(buff));
    fclose(archivo);
    close(fd[1][1]);
  }
  return 0;
}
