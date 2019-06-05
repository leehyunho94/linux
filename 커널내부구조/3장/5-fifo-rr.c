#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void check(pid_t pid)
{
    int policy;
  
    policy = sched_getscheduler(pid);
    switch(policy)
    {
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        default:
            printf("implicit......\n");
            break;
    }

}

int main(int argc, char **argv)
{
    int ret;
    struct sched_param sp = { .sched_priority = 50 };

    getpid();
    printf("trying to change the policy to SCHED_FIFO.........\n");   
    ret = sched_setscheduler(0, SCHED_FIFO, &sp);
    if(ret == -1) {
        perror("sched_setscheduler\n");
        return -1;
    }
    check(getpid());

    printf("trying to change the policy to SCHED_RR.........\n");   
    ret = sched_setscheduler(0, SCHED_RR, &sp);
    if(ret == -1) {
        perror("sched_setscheduler\n");
        return -1;
    }
    check(getpid());

    return 0;
}
