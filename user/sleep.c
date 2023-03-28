#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc,char *argv)
{
    if(argc < 2){
        fprintf(2, "Usage:lack argument 没有写时间参数啦！\n");
        exit(1);
    }
    if(argc > 2){
        fprintf(2, "Usage:overflow argument 参数多啦！\n");
        exit(1);
    }
    sleep(atoi(&argv[1])); //char->int

    exit(0);
}