#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *child_thread_main(void *arg)
{
    printf("Before exec\n");
    execl("/bin/ls", "-al", NULL);
    printf("After exec\n");

    return NULL;
}

int main(int argc, char **argv) 
{    
    int ret;
    pthread_t child_thread;
    
    ret = pthread_create(&child_thread, NULL, child_thread_main, NULL);
    if(ret) {
        printf("pthread_create(%d)\n", ret);
        return -1;
    }
    
    ret = pthread_join(child_thread, NULL);
    if(ret) {
        printf("pthread_join(%d), str_err:%s\n", 
                ret, strerror(ret));
        return -1;
    }

    printf("Done..\n");
    return 0;
}
