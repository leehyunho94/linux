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
        printf("Before exec()\n");
        execl("/bin/ls", "-al", NULL);
        _exit(0);
    } else if(pid > 0) {
        wait(&exit_status);
        if (WIFEXITED(exit_status)) {
            printf("child exited, status=%d\n", WEXITSTATUS(exit_status));
        } 
    }

    printf("After exec()\n");
    printf("Done...\n");

    return 0;
}
