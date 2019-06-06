#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#if 0
int stat(const char *pathname, struct stat *buf);
struct stat {
    dev_t     st_dev;         /* ID of device containing file */
    ino_t     st_ino;         /* inode number */
    mode_t    st_mode;        /* protection */
    nlink_t   st_nlink;       /* number of hard links */
    uid_t     st_uid;         /* user ID of owner */
    gid_t     st_gid;         /* group ID of owner */
    dev_t     st_rdev;        /* device ID (if special file) */
    off_t     st_size;        /* total size, in bytes */
    blksize_t st_blksize;     /* blocksize for filesystem I/O */
    blkcnt_t  st_blocks;      /* number of 512B blocks allocated */

    /* Since Linux 2.6, the kernel supports nanosecond
       precision for the following timestamp fields.
       For the details before Linux 2.6, see NOTES. */
#endif

int main(int argc, char **argv) 
{   
    struct stat statbuf;
    int uid, cur_prio, ret;

    if(argc < 2) {
        printf("usage : %s filename\n", argv[0]);
        return -1;
    } 

    if(stat(argv[1], &statbuf)) {
        perror("stat\n");
        return -1;
    }
    
    printf("try to check uid......\n");
    uid = statbuf.st_uid;
    if(uid == 0) { // root
        printf("try to check priority.......\n");
        cur_prio = getpriority(PRIO_PROCESS, getpid());
        
        printf("try to change to High priority.....\n");
        if(cur_prio > -19) {
            ret = setpriority(PRIO_PROCESS, getpid(), cur_prio -2);
            if(ret) {
                perror("setpriority\n");
                return -1;
            }
            printf("Change success......\n");
            printf("priority :%d\n", getpriority(PRIO_PROCESS, getpid()));
        }
        else {
            printf("current priority is higher than -19\n");
            return -1;
        }
    } else {
        printf("This file is not root account\n");
        return -1;
    }

    return 0;
}
