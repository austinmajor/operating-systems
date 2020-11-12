#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex1;

int value = 10; // this data is shared by thread(s)

void *increase(void *param); // a thread
void *decrease(void *param); // another thread

int main(int argc, char *argv[])
{
    pthread_t tid_1; // thread identifier
    pthread_t tid_2; // another thread identifier

    pthread_mutex_init(&mutex, 0);  // mutex lock
    pthread_mutex_init(&mutex1, 0); // another mutex lock

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
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex1);
    value++; //critical section
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex1);
    pthread_exit(0);
}

void *decrease(void *param)
{
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex);
    value--; //critical section
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}