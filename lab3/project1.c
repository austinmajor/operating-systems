#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUMBER_OF_THREADS 11
#define PUZZLE_SIZE 9

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
    parameters* data = (parameters *) malloc(sizeof(parameters));
    data->row = 1;
    data->col = 1;
    data->thread_number = 0;
    pthread_create(&threads[0], 0, col_valid, data);

    data = (parameters *) malloc(sizeof(parameters));
    data->row = 1;
    data->col = 1;
    data->thread_number = 1;
    pthread_create(&threads[1], 0, row_valid, data);

    int thread_number = 2;
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

    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        pthread_join(threads[i], 0);
    }

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

    for(int i = 1; i < PUZZLE_SIZE+1; ++i){
        int each_col_result[PUZZLE_SIZE+1] = {0};
        
        for(int j = 1; j <PUZZLE_SIZE+1; ++j){
            each_col_result[puzzle[j][i]] = 1;
        }

        int one_col_good = 1;
        for(int j = 1; j < PUZZLE_SIZE+1; ++j){
            if(each_col_result[j] == 0){
                one_col_good = 0;
                break;
            }
        }

        if(one_col_good){
            col_result[i] = 1;
        } else {
            printf("col %d is bad. \n", i);
        }
    }

    int all_col_good = 1;
    for(int i = 1; i < PUZZLE_SIZE + 1; ++i){
        if(col_result[i] == 0){
              all_col_good = 0;
              break;
        }
    }

    if(all_col_good){
        final_result[position->thread_number] = 1;
    }

    pthread_exit(0);
}

void* row_valid(void* param){
    parameters* position = (parameters*)param;
    int row_result[PUZZLE_SIZE+1] = {0};

    for(int i = 1; i < PUZZLE_SIZE+1; ++i){
        
        int each_row_result[PUZZLE_SIZE+1] = {0};
        for(int j = 1; j <PUZZLE_SIZE+1; ++j){
            each_row_result[puzzle[i][j]] = 1;
        }

        int one_row_good = 1;
        for(int j = 1; j < PUZZLE_SIZE+1; ++j){
            if(each_row_result[j] == 0){
                one_row_good = 0;
                break;
            }
        }

        if(one_row_good){
            row_result[i] = 1;
        } else {
            printf("row %d is bad. \n", i);
        }
    }

    int all_row_good = 1;
    for(int i = 1; i < PUZZLE_SIZE + 1; ++i){
        if(row_result[i] == 0){
              all_row_good = 0;
              break;
        }
    }

    if(all_row_good){
        final_result[position->thread_number] = 1;
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
    } else {
        printf("Region (%d, %d) is bad. \n", position->row, position->col);
    }

    pthread_exit(0);
}