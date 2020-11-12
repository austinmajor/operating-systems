#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

char p1_choice[10] = {'-','-','-','-','-','-','-','-','-','-'};
char p2_choice[10] = {'-','-','-','-','-','-','-','-','-','-'};

int* p_one_ptr;
int* p_two_ptr;
int* p_three_ptr;
const char *rps[3];

int player_one_score;
int player_two_score;
int judge;

pthread_t tid[2];
pthread_mutex_t lock[2];
pthread_cond_t cond[2];

int num_rounds = 5;

int main(int argc, char** argv) {
    num_rounds = atoi(argv[1]);

    int parent_id = getpid();

    pthread_mutex_init(&lock[0], NULL);
    pthread_mutex_init(&lock[1], NULL);
    pthread_mutex_init(&lock[2], NULL);

    rps[0] = "Rock";
    rps[1] = "Paper";
    rps[2] = "Scissors";

    player_one_score = 0;
    player_two_score = 0;

    pthread_create(&tid[0], NULL, p1, NULL);
    pthread_create(&tid[1], NULL, p2, NULL);
    pthread_create(&tid[2], NULL, judge, NULL);
    

    if(getpid() == parent_id) {
    
        for(int i=0; i<num_rounds; i++) {
        pthread_mutex_lock(&lock[0]);
        pthread_mutex_lock(&lock[1]);
        pthread_mutex_lock(&lock[2]);

        pthread_cond_signal(&cond[0]);
        pthread_cond_signal(&cond[1]);
        pthread_cond_signal(&cond[2]);
        
        while(){
        }
        pthread_mutex_unlock(&lock[0]);
        pthread_mutex_unlock(&lock[1]);
        pthread_mutex_unlock(&lock[2]);
        }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
  }

  return 0;
}

char p1(){
        // loop over arrays and input random rps value 1,2,3
    // for () {
    //     srand 
    // }
}

char p2(){
        // loop over arrays and input random rps value 1,2,3
    // for () {
    //     srand 
    // }
}

char judge(){

}