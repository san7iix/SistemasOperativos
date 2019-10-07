#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#define MAX_BUFF 1024

int main()
{
  int fd[2], n;
  char buff[MAX_BUFF];
  pipe(fd);
  int i;
  for (i = 0; i < 2; i++)
  {
    if (fork() == 0)
    {
      break;
    }
  }
  if (i == 2)
  {
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
  }
  else if (i == 1)
  {
    close(fd[1]);
    while ((n = read(fd[0], buff, MAX_BUFF)) > 0)
    {
      buff[n] = '\0';
      printf("%d [Hijo 1] Lee: %s", getpid(), buff);
    }
    close(fd[0]);
  }
  else
  {
    close(fd[0]);
    FILE *archivo;
    archivo = fopen("archivo.txt", "r");
    fgets(buff, MAX_BUFF, archivo);
    while (!feof(archivo))
    {
      write(fd[1], buff, sizeof(buff));
      fgets(buff, MAX_BUFF, archivo);
    }
    printf("%d [Hijo2] Escribio \n", getpid());
    fclose(archivo);
    close(fd[1]);
  }
  return 0;
}
