#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAXN 1024


void task(char* argv[])
{  
    int pid;
    if ( (pid=fork()) == 0)
    {
        exec(argv[0], argv);
        exit(0);
    }

    return ;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("please input param!\n");
        exit(0);
    }

    char* _argvs[MAXARG] = {0};
    int _argvcnts = 0;
    for (int i=1; i<argc; i++)
    {
        _argvs[i-1] = argv[i];
        _argvcnts++;
    }
    
    char buf[MAXN];
    int cnt = _argvcnts;
    char* ptr = buf;
    char* begin = ptr;
    char* end = ptr;
    
    while (read(0, ptr, 1) != 0)
    {
        if ( *ptr == '\n' || *ptr == ' ')
        {
            char t = *ptr;
            *ptr = 0;

            // 将buff中的位置映射到 _argvs 中s
            _argvs[cnt++] = begin;

            begin = end + 1;
            end = begin;


            if (t == '\n')
            {
                _argvs[cnt+1] = 0;
                task(_argvs);
                cnt = _argvcnts;
                begin = buf;
                end = buf;
            }

        }
        ptr++;
        end = ptr;
    }


    while (wait(0) != -1)
        ;

    exit(0);
}