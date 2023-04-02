#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"


int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        printf("please input 2 param\n");
        exit(0);
    }

    if (trace(atoi(argv[1])) < 0)
    {
        printf("%s: trace failed\n", argv[0]);
        exit(0);
    }

    char* nargv[MAXARG];
    for (int i=2; i<argc && i<MAXARG; i++)
    {
        nargv[i-2] = argv[i];
    }

    exec(nargv[0], nargv);

    exit(0);
}