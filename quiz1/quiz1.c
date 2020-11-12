// CS 540 quiz 1: use multithread for collatz program. 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define MAX_CAP 100

int nums[MAX_CAP] = {0}; 
int count = 0; 
int n = 35; 

void *calc(void *param); 
void *print(void *param); 

int main(int argc, char *argv[])
{
	pthread_t tid_1; 
	pthread_create(&tid_1, 0, calc, 0);
	pthread_join(tid_1, NULL);

    pthread_t tid_2; 
	pthread_create(&tid_2, 0, print, 0);
	pthread_join(tid_2, NULL);

	return 0;
}

void *calc(void *param){
	nums[count++] = n; 	
	while(n != 1){
		nums[count++] = (n % 2 == 0) ? (n = n /2) : (n = 3 * n + 1); 
	}
	pthread_exit(0);
}

void *print(void *param){
	for(int i = 0; i < count; ++i)
		printf("%d ", nums[i]);
	printf("\n");
	pthread_exit(0);
}