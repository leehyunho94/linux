#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define GROUP1 101
#define GROUP2 102

int main(int argc, char **argv)
{
    int ret, exit_status;
    pid_t pid;
  
    ret = setgid(GROUP1);
    if(ret) {
        perror("ret()\n");
        printf("err(%d):%s\n", ret, strerror(ret));
        return -1;
    } 

    for(int i=0; i<6; i++) {
        pid = fork();
        if(pid == 0) {
            printf("PID:%d, GRP_ID:%d\n", getpid(), getgid());
            sleep(10);
            _exit(0);
        }
    }
   
    for(int i=0; i<3; i++) {
        pid = fork();
        if(pid == 0) {
            setgid(GROUP2);
            printf("PID:%d, GRP_ID:%d\n", getpid(), getgid());
            sleep(10);
            _exit(0);
        }
    }
    
    for(int i=0; i<9; i++) {
        wait(&exit_status);
    }

    printf("Done...\n");
    return 0;
}
