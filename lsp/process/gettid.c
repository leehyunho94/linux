#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _GNU_SOURCE         
#include <sys/syscall.h>
#include <sys/types.h>

int main(int argc, char **argv) 
{
    pid_t tid;
    tid = syscall(SYS_gettid); // tid = syscall(__NR_gettid);    
    printf("tid:%d\n", tid);
    return 0;
}
