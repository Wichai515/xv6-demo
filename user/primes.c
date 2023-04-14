#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define YES '1'
#define NO '0'

void prime(int p_read,int p_write)
{
    char buf[36];
    read(p_read,buf,36);
    int i,value = 0;
    for(i = 0; i < 36; i ++)
    {
        //找到第一个素数
        if(buf[i] == YES)
        {
            value = i;
            break;
        }
    }
    if(value == 0) exit(0);
    printf("prime %d\n",value);
    //printf("到for了");
    //划去他的倍数
    buf[value] = NO;
    for(i = 0; i < 36; i ++)
    {
        if(i % value == 0)
        {
            buf[i] = NO;
        }
    }
    //printf("%s\n",buf);
    
    //子节点处理完毕，fork给孙节点
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }else if(pid > 0 )
    {
        //当前的子节点（孙的父）
        write(p_write,buf,36);
        //printf("zi %s\n",buf);
    }else if(pid == 0)
    {
        //孙节点
        //printf("sun %s\n",buf);
        prime(p_read,p_write);
    }
}

int main(int argc, char *argv[])
{
    //printf("Starting primes program...\n");
    int pp[2];
    pipe(pp);

    char n[36];
    int i;
    for(i = 2; i < 36; i ++)
    {
        n[i] = YES;
    }

    int pid = fork();
    if(pid > 0)
    {
        //parent
        n[0] = NO;
        n[1] = NO;
        write(pp[1],n,36);
        wait(0); // wait pid == 0 exit

    }else if(pid == 0){
        //child
        prime(pp[0],pp[1]);
        
    }
    wait(0);
    exit(0);
}