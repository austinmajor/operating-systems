#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void collatz(int n)
{
    printf("%d ", n);
    if (n == 1)
        return;
    if (n % 2 == 0)
        collatz(n / 2);
    else
        collatz(3 * n + 1);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No argument was passed\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n <= 0)
    {
        printf("Provide a positive input\n");
        exit(1);
    }

    if (fork() == 0)
    {
        if (fork() == 0)
        {
            collatz(n);
            printf("\n");
        }
    }
    else
    {
        wait(NULL);
    }
}