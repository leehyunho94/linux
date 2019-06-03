#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    int ret, exit_status;
    pid_t pid;
    
    pid = fork();
    if(pid == 0) {
        ret = execl("/bin/ls", "-a", "-l", NULL);
        if(ret == -1) {
            perror("execl()\n");
            return -1;
        }
    } else if(pid > 0) {
        wait(&exit_status);
        if (WIFEXITED(exit_status)) {
            printf("exited, status=%d\n", WEXITSTATUS(exit_status));
        }
    } else {
        perror("fork()\n");
        return -1;
    }

    return 0;
}
