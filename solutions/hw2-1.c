/* CS 540 hw 2: create a child process that enters zombie status. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
pid_t pid;
pid = fork(); /* fork a child process */
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed\n");
return 1;
}
else if (pid == 0) { /* child process */
exit(0);
}
else { /* parent process */
sleep(15);
wait(0);
}
return 0;
}