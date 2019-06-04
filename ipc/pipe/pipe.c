#define _POSIX_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    pid_t pid;
    int p[2];
    char c = '?';

    if(pipe(p) != 0){
        perror("pipe() error\n");
    }
    else {
        if((pid = fork()) ==0) {
            printf("child process group id is %d\n",
                    (int)getpgrp());
            write(p[1], &c, 1);
            setsid();
            printf("child process group id is now %d\n",
                    (int)getpgrp());
            _exit(0);
        } 
        else {
            printf("parent process group id is %d\n",
                    (int)getpgrp());
            read(p[0], &c, 1);
            sleep(5);
        }
    }
    return 0;
}
