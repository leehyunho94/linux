#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int exit_status;
    pid_t pid;

    pid = fork();
    if(pid == 0) {
        sleep(3);
        printf("Before exit()\n");
        _exit(0);
    } else if(pid > 0) {
        wait(&exit_status);
        if (WIFEXITED(exit_status)) {
            printf("child exited, status=%d\n", WEXITSTATUS(exit_status));
        }
    } else {
        perror("fork()\n");
        return -1;
    }
    
    printf("After exit()\n");
    printf("Done...\n");

    return 0;
}
