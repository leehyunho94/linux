#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>     
#include <sys/stat.h>  
#include <semaphore.h>

#define SEM_ID  "/test_sem_id"

static void print_usage(char *progname)
{
    printf("usage : %s (show|wait|trywait|timedwait|release)\n", 
            progname);
    return;
}

static sem_t *init_semaphore(void)
{
    sem_t *sem;

    sem = sem_open(SEM_ID, O_CREAT, 0644, 3);
    if(sem == SEM_FAILED) {
        perror("sem_open()");
        return NULL;
    }

    return sem;
}

static int sem_posix_show(sem_t *sem)
{
    int i;

    if(sem_getvalue(sem, &i) == -1) {
        perror("sem_getvalue()");
        return -1;
    }

    printf("semaphore : %d\n", i);
    return 0;
}

static int sem_posix_wait(sem_t *sem)
{
    return sem_wait(sem);
}

static int sem_posix_trywait(sem_t *sem)
{
    return sem_trywait(sem);
}

static int sem_posix_timedwait(sem_t *sem)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 3;
    return sem_timedwait(sem, &ts);
}

static int sem_posix_release(sem_t *sem)
{
    return sem_post(sem);
}

int main(int argc, char **argv) 
{
    int ret;
    sem_t *sem;

    if(argc < 2) {
        print_usage(argv[0]);
        return -1;
    }

    sem = init_semaphore();
    if(sem == NULL) {
        perror("init semaphore");
        return -1;
    }

    if(!strcmp(argv[1], "show")) {
       ret = sem_posix_show(sem); 
    } else if(!strcmp(argv[1], "wait")) {
       ret = sem_posix_wait(sem); 
    } else if(!strcmp(argv[1], "trywait")) {
       ret = sem_posix_trywait(sem); 
    } else if(!strcmp(argv[1], "timedwait")) {
       ret = sem_posix_timedwait(sem); 
    } else if(!strcmp(argv[1], "release")) {
       ret = sem_posix_release(sem); 
    } else {
        print_usage(argv[0]);
        return -1;
    }
    return ret; 
}
