#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int exit_status;
    pid_t pid;
    struct stat buf;

    memset(&buf, 0, sizeof(struct stat));

    printf("parent process:%d\n", getpid());    
    for(int i=0; i<2; i++) {
        pid = fork();
        if(pid == 0) {
            int n = 5;
            while(n--) {
                // stat("/proc/`getpid()`/stat", &buf);
                // printf("gid:%d pid:%d\n", (int)buf.st_gid, (int)getpid());
                printf("pid:%d\n", getpid());
                sleep(1);
            }
            _exit(0);
        }
        else if(pid > 0) {
            wait(&exit_status);
        }        
    }

    printf("Done...\n");
    return 0;
}
