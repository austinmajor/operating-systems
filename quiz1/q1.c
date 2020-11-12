#include<pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define NUMBER_OF_THREADS 2

int final_result[NUMBER_OF_THREADS];

typedef struct{
    int number;
    int thread_number;
} parameters;

void* collatz(void* n)
{
    parameters* n = (parameters*)n;
    printf("%d ", n);
    //pthread_create(&threads[1], 0, printf, "%d" n);
    if (n->number == 1)
        return;
    if (n->number % 2 == 0)
        collatz(n->number / 2);
    else
        collatz(3 * n->number + 1);
}

int main()
{
    pthread_t threads[NUMBER_OF_THREADS];
    parameters* data = (parameters *) malloc(sizeof(parameters));
    data->number = 35;
    data->thread_number = 0;

    pthread_create(&threads[0], 0, collatz, data);
    
    for(int i = 0; i < NUMBER_OF_THREADS; ++i) { 
        pthread_join(threads[i], 0);
    }

    return 0;
}