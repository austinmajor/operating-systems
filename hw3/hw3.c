#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUMBER_OF_THREADS 27
#define PUZZLE_SIZE 9

//Puzzle size has +1 border (-1 used) to match numbers (1-9) used in sudoku
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,5,3,4,6,7,8,9,1,2},
            {-1,6,7,2,1,9,5,3,4,8},
            {-1,1,9,8,3,4,2,5,6,7},
            {-1,8,5,9,7,6,1,4,2,3},
            {-1,4,2,6,8,5,3,7,9,1},
            {-1,7,1,3,9,2,4,8,5,6},
            {-1,9,6,1,5,3,7,2,8,4},
            {-1,2,8,7,4,1,9,6,3,5},
            {-1,3,4,5,2,8,6,1,7,9}
};

int final_result[NUMBER_OF_THREADS] = {0};

void* row_valid(void* param);
void* col_valid(void* param);
void* region_valid(void* param);

typedef struct{
    int row;
    int col;
    int thread_number;
} parameters;

int main(){
    pthread_t threads[NUMBER_OF_THREADS];

    //Check columns (i,j):
    //(i,j) = (row,column)
    //(1,1) - (1,9)
    //(2,1) - (2,9)
    //(3,1) - (3,9)
    //(4,1) - (4,9)
    //(5,1) - (5,9)
    //(6,1) - (6,9)
    //(7,1) - (7,9)
    //(8,1) - (8,9)
    //(9,1) - (9,9)
    parameters* data = (parameters *) malloc(sizeof(parameters));
    int thread_number = 0;
    for(int i = 1; i < PUZZLE_SIZE+1; ++i){
        data = (parameters *) malloc(sizeof(parameters));
        data->row = 1;
        data->col = i;
        data->thread_number = thread_number;
        pthread_create(&threads[thread_number], 0, col_valid, data);
        thread_number++;
    }
    
    //Check rows (i,j):
    //(i,j) = (row,column)
    //(1,1) - (9,1)
    //(2,1) - (9,2)
    //(3,1) - (9,3)
    //(4,1) - (9,4)
    //(5,1) - (9,5)
    //(6,1) - (9,6)
    //(7,1) - (9,7)
    //(8,1) - (9,8)
    //(9,1) - (9,9)
    for(int i = 1; i < PUZZLE_SIZE+1; ++i){
        data = (parameters *) malloc(sizeof(parameters));
        data->row = i;
        data->col = 1;
        data->thread_number = thread_number;
        pthread_create(&threads[thread_number], 0, row_valid, data);
        thread_number++;
    }

    //Check regions (i,j):
    //(i,j) = (row,column)
    //(1,1) - (1,4) - (1,7) 
    //(4,1) - (4,4) - (4,7)
    //(7,1) - (7,4) - (7,7)
    for(int i = 1; i < PUZZLE_SIZE + 1; i+=3){
        for(int j = 1; j < PUZZLE_SIZE + 1; j+=3){
                data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                data->col = j;
                data->thread_number = thread_number;
                pthread_create(&threads[thread_number], 0, region_valid, data);
                thread_number++;
        }
    }

    //Wait for all threads to finish
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        pthread_join(threads[i], 0);
    }

    //Verdict check if all threads are valid
    int verdict = 1;
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        if(final_result[i] == 0){
            verdict = 0;
            break;
        }
    }

    if(verdict){
        printf("Sudoku is valid! \n");
    } else {
        printf("Sudoku is Not valid! \n");
    }

    return 0;
}

void* col_valid(void* param){
    parameters* position = (parameters*)param;

    int col_result[PUZZLE_SIZE+1] = {0};
    for(int i = 1; i <PUZZLE_SIZE+1; ++i){
        col_result[puzzle[i][position->col]] = 1;
    }

    int col_good = 1;
    for(int j = 1; j < PUZZLE_SIZE+1; ++j){
        if(col_result[j] == 0){
            col_good = 0;
            break;
        }
    }

    if(col_good){
        final_result[position->thread_number] = 1;
        printf("col %d is good, %d \n", position->col, position->thread_number);
    } else {
        printf("%d col is bad, %d \n", position->col, position->thread_number);
    }

    pthread_exit(0);
}

void* row_valid(void* param){
    parameters* position = (parameters*)param;

    int row_result[PUZZLE_SIZE+1] = {0};
    for(int i = 1; i <PUZZLE_SIZE+1; ++i){
        row_result[puzzle[position->row][i]] = 1;
    }

    int row_good = 1;
    for(int j = 1; j < PUZZLE_SIZE+1; ++j){
        if(row_result[j] == 0){
            row_good = 0;
            break;
        }
    }

    if(row_good){
        final_result[position->thread_number] = 1;
        printf("row %d is good, %d \n", position->row, position->thread_number);
    } else {
        printf("%d row is bad, %d \n", position->row, position->thread_number);
    }
 
    pthread_exit(0);
}

void* region_valid(void* param){
    parameters* position = (parameters*)param;

    int each_region[PUZZLE_SIZE+1] = {0};
    for(int i = position->row; i < position->row + 3; ++i){
        for(int j = position->col; j < position->col + 3; ++j){
            each_region[puzzle[i][j]] = 1;
        }
    }

    int one_region_good = 1;
    for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
        if(each_region[j] == 0){
            one_region_good = 0;
            break;
        }
    }

    if(one_region_good){
        final_result[position->thread_number] = 1;
        printf("Region (%d, %d) is good, %d \n", position->row, position->col, position->thread_number);
    } else {
        printf("(%d, %d) is bad, %d \n", position->row, position->col, position->thread_number);
    }

    pthread_exit(0);
}