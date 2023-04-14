#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main()
{
    int pid = fork();
    if(pid > 0){
        printf("parent: child=%d\n", pid); 
        pid = wait((int * ) 4); 
        printf("child %d is done\n", pid);
    } else if(pid == 0){
        printf("child: exiting\n");
        exit(0); 
    } else {
        printf("fork error\n");
    }
    exit(0);
}


// output:
// parent: child=1234
// child: exiting
// parent: child 1234 is done