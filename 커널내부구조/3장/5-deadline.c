#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <linux/sched.h>
#include <sys/syscall.h>

struct sched_attr {
    uint32_t size;              /* Size of this structure */
    uint32_t sched_policy;      /* Policy (SCHED_*) */
    uint64_t sched_flags;       /* Flags */
    uint32_t sched_nice;        /* Nice value (SCHED_OTHER,
                              SCHED_BATCH) */
    uint32_t sched_priority;    /* Static priority (SCHED_FIFO,
                              SCHED_RR) */
    /* Remaining fields are for SCHED_DEADLINE */
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
};

int sched_setattr(pid_t pid, const struct sched_attr *attr, uint32_t flags)
{
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

void check(pid_t pid)
{
    int policy;
  
    policy = sched_getscheduler(pid);
    switch(policy)
    {
        case SCHED_OTHER:
            printf("SCHED_OTHER\n");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        case SCHED_DEADLINE:
            printf("SCHED_DEADLINE\n");
            break;
        default:
            printf("implicit......\n");
            break;
    }

}

int main(int argc, char **argv) 
{
    int ret;
    struct sched_attr attr = {
        .size = sizeof(attr),
        .sched_policy = SCHED_DEADLINE,
        .sched_runtime = 30000000,
        .sched_period = 100000000,
        .sched_deadline = 100000000
    };
    
    check(getpid());

    printf("trying to change SCHED_DEADLINE.....\n");    
    ret = sched_setattr(getpid(), &attr, 0);
    if(ret) {
        perror("sched_setattr\n");
        return -1;
    }

    check(getpid());
    return 0;
}
