#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
   p++;
  strcpy(buf, p);
  buf[DIRSIZ] = '\0';
  return buf;
}

void
find(char *path,char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path,0)) < 0){
        fprintf(2, "find: cannot open %s\n",path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n",path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(strcmp(name, fmtname(path)) == 0){   //注意path 和 buf
                printf("%s\n", path);
            }
            break;
        
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/' ;
            while (read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                //jump***
                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;// 跳过当前目录（.）和上一级目录（..）
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0'; // !!!
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n",buf);
                    continue;
                }
                find(buf, name); //递归***
 // printf("%d %d %d\n",strcmp(name,fmtname(buf)),strlen(name),strlen(fmtname(buf)));
// printf("%s/%s\n", path,fmtname(buf));
            }
        break;
    }
    close(fd);//****  为什么要关？防止资源泄漏（不确定）
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2,"lack of argument\n");
        exit(0);
    }
    char *path = argv[1];
    char *name = argv[2];
    find(path,name);
    exit(0);
}