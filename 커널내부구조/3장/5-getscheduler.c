#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*
struct sched_param {
    ...
    int sched_priority;
    ...
};
*/

int main(int argc, char **argv)
{
    pid_t pid;
    int policy;

    pid = getpid();
    printf("Scheduler Policy for PID: %d -> ", pid);
    policy = sched_getscheduler(pid);
    
    switch(policy) {
        case SCHED_OTHER:
            printf("SCHED_OTHER\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        default:
            printf("Unknown...\n");
            break;
    } 

    return 0;
}
