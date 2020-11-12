#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int value = 10; // this data is shared by thread(s)

void *increase(void *param); // a thread
void *decrease(void *param); // another thread

int main(int argc, char *argv[])
{
    pthread_t tid_1;
    pthread_t tid_2;

    while (1)
    {
        pthread_create(&tid_1, 0, increase, 0);
        pthread_create(&tid_2, 0, decrease, 0);

        pthread_join(tid_1, NULL);
        pthread_join(tid_2, NULL);

        printf("value = %d\n", value);
    }
}

void *increase(void *param)
{
    value++;
    pthread_exit(0);
}

void *decrease(void *param)
{
    value--;
    pthread_exit(0);
}