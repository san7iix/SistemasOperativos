#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_BUFF 1024

int main(int argc, char const *argv[])
{

    char buff[MAX_BUFF];
    int i = 0;
    int tuberia[6][2],n;
    pid_t padre = getpid();

    for (int j = 0; j < 12; j++)
    {
        pipe(tuberia[j]);
    }

    for (; i < 2; i++)
    {
        if (fork() == 0)
        {
            break;
        }
    }

    if (i == 2)
    {
        for (int k = 1; k < 6; k++){
            for (int x = 0; x < 2; x++){
                close(tuberia[k][x]);
            }
        }
        close(tuberia[0][0]);
        printf("[%d] Proceso padre\n", getpid());
        strcpy(buff,"1");
        write(tuberia[0][1], buff, sizeof(buff));
        close(tuberia[0][1]);
        for (i = 0; i < 2; i++)wait(NULL);
    }
    else
    {
        sleep(1);
        if (i == 0)
        {
            //Hijo 1
            if (fork() == 0)
            {
                if (fork() == 0)
                {
                    //Nieto 1
                    //Codigo Nieto 1-1-1
                    // for (int k = 1; k < 6; k++){
                    //     for (int x = 0; x < 2; x++){
                    //         close(tuberia[k][x]);
                    //     }
                    // }
                }
                else
                {
                    //Codigo Hijo 1-1
                }
            }
            else
            {
                //Codigo hijo 1
                for (int k = 2; k < 6; k++){
                    for (int x = 0; x < 2; x++){
                        close(tuberia[k][x]);
                    }
                }
                close(tuberia[0][1]);
                close(tuberia[1][0]);
                while ((n = read(tuberia[0][0], buff, MAX_BUFF)) > 0){
                    buff[n] = '\0';
                    printf("\n[%d][Hijo 1] Lee: %s \n", getpid(), buff);
                }
                close(tuberia[1][1]);
            }
        }
        else if (i == 1)
        { //Hijo 2
        }
    }
}