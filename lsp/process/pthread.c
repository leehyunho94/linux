#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/unistd.h>

int *ptr;
int flag = 1;

void *child_func(void *arg)
{
    int base = flag ? 123 : 456;

    printf("TGID(%d), PID(%d)\n", (int)getpid(), (int)syscall(__NR_gettid));

    if(flag) {
        ptr = &base;
        flag = 0;
        printf("base1:%d\n", *ptr);
    } else {
        printf("base2:%d\n", *ptr);
    }
    
    // sleep(2);
    return NULL;
}

int main(int argc, char **argv)
{
    int pid, ret;
    
    pthread_t pthread_child[2];
    printf("before pthread_create\n");
    pid = pthread_create(&pthread_child[0], NULL, child_func, NULL);
    if(pid < 0) {
        perror("pthread[0] error\n");
        return -1;
    }

    pid = pthread_create(&pthread_child[1], NULL, child_func, NULL);
    if(pid < 0) {
        perror("pthread[0] error\n");
        return -1;
    }
    
    ret = pthread_join(pthread_child[0], NULL);
    if(ret) {
        perror("pthread[0] join error\n");
        return -1;
    }
    
    ret = pthread_join(pthread_child[1], NULL);
    if(ret) {
        perror("pthread[1] join error\n");
        return -1;
    }

    return 0;
}

