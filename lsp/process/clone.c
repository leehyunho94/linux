#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/unistd.h>

int *ptr;
int flag = 1;

int sub_func(void *arg)
{
    int base = flag ? 123 : 456;

    if(flag) {
        ptr = &base;
        flag = 0;
        printf("1:%d\n", *ptr);
    } else {
        printf("2:%d\n", *ptr);
    }
    
    return 0;
}

int main(int argc, char **argv) 
{
    int ret, child_astack[4096], child_bstack[4096];
    
    ret = clone(sub_func, (void*)(child_astack+4095), CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID, NULL);
    if(ret) {
        perror("clone 0 fail\n");
        return -1;
    }

    ret = clone(sub_func, (void*)(child_bstack+4095), CLONE_VM | CLONE_THREAD | CLONE_SIGHAND, NULL);
    if(ret) {
        perror("clone 1 fail\n");
        return -1;
    }
    
    return 0;
}
