#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CAP 100

int nums[MAX_CAP] = {0};
int count = 0;

int main(int argc, char *argv[])
{
    pid_t pid, pid1;
    int n;
    if (argc == 1)
    {
        fprintf(stderr, "Usage: ./a.out <starting value>\n");
        return -1;
    }
    n = atoi(argv[1]);
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Unable to fork child\n");
        return -1;
    }
    else if (pid == 0)
    { /* child process (2nd process), store numbers into the array nums[]*/
        nums[count++] = n;
        while (n != 1)
        {
            nums[count++] = (n % 2 == 0) ? (n = n / 2) : (n = 3 * n + 1);
        }
        pid1 = fork(); // create third process to print all numbers
        if (pid1 < 0)
        {
            fprintf(stderr, "Unable to fork child\n");
            return -1;
        }
        else if (pid1 == 0)
        {
            for (int i = 0; i < count; ++i)
                printf("%d ", nums[i]);
            printf("\n");
        }
        else
        {
            wait(NULL);
        }
    }
    else
    { /* parent process (1st process) */
        wait(NULL);
    }
    return 0;
}