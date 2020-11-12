// Sudoku checker

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// need to have 1 thread to check all rows 
//              1 thread to check all columns 
//              9 threads to check all 9 regions (one thread for each region)
#define NUMBER_OF_THREADS  11 

#define PUZZLE_SIZE 9 

int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,5,3,4,6,7,8,9,1,2},             // 5,3,4,6,7,8,9,1,2
			{-1,6,7,2,1,9,5,3,4,8},             // 6,7,2,1,9,5,3,4,8
			{-1,1,9,8,3,4,2,5,6,7},
			{-1,8,5,9,7,6,1,4,2,3},             // 8,5,9,7,6,1,4,2,3
			{-1,4,2,6,8,5,3,7,9,1},
			{-1,7,1,3,9,2,4,8,5,6},
			{-1,9,6,1,5,3,7,2,8,4},
			{-1,2,8,7,4,1,9,6,3,5},
			{-1,3,4,5,2,8,6,1,7,9}
		};

// each thread will report to the final_result
// final_result will be used to make the verdict
int final_result[NUMBER_OF_THREADS] = {0}; 

// check all 9 rows
void* row_valid     (void* param);

// check all 9 columns 
void* col_valid     (void* param);

// check a given region is valid or not
// the region is specified by param->row and param->col 
void* region_valid  (void* param);

typedef struct{ 
    int row;
    int col;
    int thread_number;     // to be used by thread to know its # 
} parameters; 

int main(){

    // create NUMBER_OF_THREADS threads 
    pthread_t threads[NUMBER_OF_THREADS];
    
    // check all columns 
    parameters* data = (parameters *) malloc(sizeof(parameters)); 
    data->row = 1; 
    data->col = 1; 
    data->thread_number = 0; 
    pthread_create(&threads[0], 0, col_valid, data);    // create the first thread to check all columns 

    // check all rows
    data = (parameters *) malloc(sizeof(parameters)); 
    data->row = 1; 
    data->col = 1; 
    data->thread_number = 1; 
    pthread_create(&threads[1], 0, row_valid, data);    // create the first thread to check all rows 

    // check all 9 regions 
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

    // wait for all threads to finish 
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        pthread_join(threads[i], 0);
    }

    //check the final_result to validate the whole sudoku puzzle
    int verdict = 1;
    for(int i = 0; i < NUMBER_OF_THREADS; ++i){
        if (final_result[i] == 0){
            verdict = 0; 
            break;
        }
    }

    if (verdict){
        printf("Sudoku is valid.\n");
    }
    else{
        printf("Sudoku is NOT valid. \n");
    }

    return 0;
}

void* col_valid(void* param){
    parameters* position = (parameters*)param;  

    // result for all 9 columns 
    // 1 means that col is good. 
    int col_result[PUZZLE_SIZE + 1] = {0};

    // a loop to check all 9 columns 
    for (int i = 1; i <PUZZLE_SIZE + 1; ++i){

        // check the existence of all nine numbers in one column  
        int each_col_result[PUZZLE_SIZE + 1] = {0};
        for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
            each_col_result[puzzle[j][i]] = 1;          // it is [j][i] for col, not [i][j] (for row)
        }

        int one_col_good = 1; 
        // check this col 
        // check to see if all nine numbers are registered into the each_col_result array
        for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
            if (each_col_result[j] == 0){
                one_col_good = 0; 
                break;
            }
        }

        if (one_col_good){
            col_result[i] = 1;
        }
        else {
            printf("%d col is bad.\n", i);
        }
    }

    // check values in col_result array, if all 1 then all cols are good. 
    int all_col_good = 1; 
    for(int i = 1; i < PUZZLE_SIZE + 1; ++i){
        if (col_result[i] == 0){
            all_col_good = 0; 
            break;
        }
    }

    if (all_col_good)
        final_result[position->thread_number] = 1;

    free(position);
    pthread_exit(0);
}

void* row_valid(void* param){
    parameters* position = (parameters*)param;  

    // result for all rows 
    // 1 means that row is good. 
    int row_result[PUZZLE_SIZE + 1] = {0};

    // a loop to check all 9 rows 
    for (int i = 1; i <PUZZLE_SIZE + 1; ++i){

        // check the existence of all nine numbers in 1 row  
        int each_row_result[PUZZLE_SIZE + 1] = {0};
        for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
            each_row_result[puzzle[i][j]] = 1;          // it is [i][j], not [j][i]
        }

        int one_row_good = 1; 
        // check this row 
        for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
            if (each_row_result[j] == 0){
                one_row_good = 0; 
                break;
            }
        }

        if (one_row_good){
            row_result[i] = 1;
        }
        else {
            printf("%d row is bad.\n", i);
        }
    }

    // check values in row_result array, if all 1 then all rows are good. 
    int all_row_good = 1; 
    for(int i = 1; i < PUZZLE_SIZE + 1; ++i){
        if (row_result[i] == 0){
            all_row_good = 0; 
            break;
        }
    }

    if (all_row_good)
        final_result[position->thread_number] = 1;
    
    free(position);
    pthread_exit(0);
}

void* region_valid(void* param){
    parameters* position = (parameters*)param;  

    int each_region[PUZZLE_SIZE + 1 ] = {0};
    for(int i = position->row; i < position->row + 3; ++i){
        for(int j = position->col; j < position->col + 3; ++j){
            each_region[puzzle[i][j]] = 1; 
        }
    }

    int one_region_good = 1; 
    for(int j = 1; j < PUZZLE_SIZE + 1; ++j){
        if (each_region[j] == 0){
            one_region_good = 0; 
            break;
        }
    }

    if (one_region_good){
        final_result[position->thread_number] = 1; 
    }
    else {
        printf("(%d, %d) is bad.\n", position->row, position->col);
    }
    free(position);
    pthread_exit(0);
}