#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

char p1_choice[] = "----------";
char p2_choice[] = "----------";

void *judge(void *param);

int main(int argc, char *argv[])
{
    srand(time(0));

    pthread_t tid_1;
    pthread_mutex_t lock[p2_choice]; 
	pthread_create(&tid_1, 0, judge, p1_choice);
	pthread_join(tid_1, NULL);

    pthread_t tid_2;
    pthread_mutex_t lock[p1_choice]; 
	pthread_create(&tid_2, 0, judge, p2_choice);
	pthread_join(tid_2, NULL);

    return (0);
}

void *generate(void *param) {
    char rsp[] = "rsp";
    char choice;
    for (int i=0; i<10; i++)
    {
        int r = rand() % 3;
        choice = rsp[r];
    }
    printf('%c\n', choice);
    pthread_exit(0);
}