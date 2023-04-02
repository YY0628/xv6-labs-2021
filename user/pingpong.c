#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    // p1[0] -> p1[1]
    // p2[0] -> p2[1]
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

    int pid;
    if ((pid=fork()) != 0)
    {
        // 父进程   写 p1[0]   读 p2[1]
        close(p1[0]);
        close(p2[1]);

        char buff;
        write(p1[1], "a", 1);

        read(p2[0], &buff, 1);
        printf("%d: received pong\n", getpid());

        wait(0);
    }
    else
    {
        // 子进程   写 p2[0]   读 p1[1]
        close(p1[1]);
        close(p2[0]);

        char buff;
        read(p1[0], &buff, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &buff, 1);
        exit(1);
    }

    exit(0);
}

// #include "kernel/types.h"
// #include "user/user.h"
// int main()
// {
//     int p1[2], p2[2];
//     pipe(p1);
//     pipe(p2);
//     int ret = fork();
//     if (ret == 0) //子进程
//     {
//         char buf;
//         close(p2[0]);         //关闭读端
//         close(p1[1]);         //关闭写端
//         read(p1[0], &buf, 1); //从父进程读
//         printf("%d: received ping\n", getpid());
//         write(p2[1], &buf, 1); //发送给子进程
//         exit(1);
//     }   
//     else //父进程
//     {
//         char buf;
//         close(p1[0]);         //关闭读端
//         close(p2[1]);         //关闭写端
//         write(p1[1], "!", 1); //发送给子进程
//         read(p2[0], &buf, 1); //接收子进程
//         printf("%d: received pong\n", getpid());
//         wait(0);
//     }
//     return 0;
// }
