#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    /* fork a child process */
    pid = fork();

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0) { /* child process */
        printf("child: pid = %d\n", pid);
        exit(0);
    } else { /* parent process - will wait on child to complete */
        printf("parent: pid = %d\n", pid);

        sleep(15);

        int state;
        wait(&state);

        printf("Child complete\n");
    }

    return 0;
}