#include "kernel/types.h"
#include "user/user.h"
#define MAXSIZE 35


void process(int p[])
{
    close(p[1]);
    int base;
    if (read(p[0], &base, sizeof(int)) > 0)
    {
        printf("prime %d\n", base);
        
        int pid;
        int subP[2];
        pipe(subP);
        if ( (pid=fork()) != 0)
        {
            close(subP[0]);
            int num;
            while (read(p[0], &num, sizeof(int)) > 0)
            {
                if (num % base != 0)
                {
                    write(subP[1], &num, sizeof(int));
                }        
            }
            close(subP[1]);
            wait(0);
        }
        else
        {
            close(p[0]);
            process(subP);
        }    
    }

}


int main(int argc, char* argv[])
{

    int p[2];
    pipe(p);

    int pid;
    if ( (pid=fork()) != 0)
    {
        close(p[0]);
        for (int i=2; i<MAXSIZE; i++)
        {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    else
    {
        process(p);
    }

    exit(0);    
}