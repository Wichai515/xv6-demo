#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[16];

int 
main(int argc,char *argv)
{
    int p[2];  //fd
    pipe(p);
    int pid = fork();
    if(pid == 0)
    {
        //child process
        //close(p[1]); // 关闭写端
        read(p[0],buf,16);
        printf("%d: recieved %s\n",getpid(),buf);
        write(p[1],"pong",16);
    }else{
        //parent process
        //close(p[0]); // 关闭读端
        write(p[1],"ping",16);
        wait((int *) 0); 
        // 不关心子进程退出状态
        //xv6的用户程序中，NULL没有定义。
        //使用(int *) 0作为wait()函数的参数
        read(p[0],buf,16);
        printf("%d: recieved %s\n",getpid(),buf);
    }
    exit(0);
}