#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUMTHREADS 4

struct mat {
    int rows;
    int cols;
    int *mat;
};

struct thread_info {
    int is_running;
    int start;
    int stop;
    struct mat *mat_a;
    struct mat *mat_b;
    struct mat *mat_result;
};

void make_mat(int rows, int cols, struct mat* arr);
void* multiply_with_threads(void* arg);
void multiply(struct mat* arr, struct mat* arr2, struct mat* result );
void fill_mat(struct mat* arr);
int sum_array(int* arr, int size);

int main() {
    int col=1000;
    int row=1000;
    int num_threads = NUMTHREADS;
    struct thread_info mat_info[num_threads];
    pthread_t thread[num_threads];
    struct mat arr, arr2, arr_result;
    make_mat(row,col,&arr);
    make_mat(row,col,&arr2);
    make_mat(row,col,&arr_result);
    fill_mat(&arr); 
    fill_mat(&arr2); 

    int rows_per_thread = ceil(double(row) / num_threads);
    printf("%d",rows_per_thread);

    // store information for threads object
    for (int i = 0; i < num_threads; i++) {
        mat_info[i].mat_a = &arr;
        mat_info[i].mat_b = &arr2;
        mat_info[i].mat_result = &arr_result;
        mat_info[i].is_running = 1;
        mat_info[i].start = i*rows_per_thread;
        mat_info[i].stop = (i+1)*rows_per_thread;
    }
    mat_info[num_threads-1].stop=row;

    /* print_mat(&arr); */
    for (int i=0; i<num_threads; i++) {
        pthread_create(&thread[i], NULL, multiply_with_threads, &mat_info[i]);
    }

    /* Wait for Threads to Finish */
    int flags[num_threads];
    while(sum_array(flags,num_threads)!=3){
        for (int i = 0; i < num_threads; i++) {
            if(mat_info[i].is_running == 0 && flags[i] != 1){ 
                printf("done\n");
                flags[i]=1;
            }
        }

    }

    /* multiply(&arr,&arr2,&arr_result); */    
    
    free(arr.mat);
    free(arr2.mat);
    free(arr_result.mat);

    return 0;
}

void make_mat(int rows, int cols, struct mat* arr){
    arr->mat = (int *)malloc(rows * cols * sizeof(int));
    arr->rows = rows;
    arr->cols = cols;
}


void* multiply_with_threads(void* arg){
    struct thread_info  *mat_info = (struct thread_info*) arg;
    int i,j,k;
    for (i=mat_info->start;i<mat_info->stop;i++){
        for (j=0;j<mat_info->mat_b->cols;j++){
            for (k=0;k<mat_info->mat_a->cols;k++){
                mat_info->mat_result->mat[i*mat_info->mat_a->cols + j] += 
                    (mat_info->mat_a->mat[i*mat_info->mat_a->cols + k] * mat_info->mat_b->mat[k*mat_info->mat_b->cols + j]);
            }
        }
    }
    mat_info->is_running = 0;
    pthread_exit(0); 
}

void multiply(struct mat* arr, struct mat* arr2, struct mat* result ){
    int i,j,k;

    for (i=0;i<arr->rows;i++){
        for (j=0;j<arr2->cols;j++){
            for (k=0;k<arr->cols;k++){
                result->mat[i*arr->cols + j] += 
                    (arr->mat[i*arr->cols + k] * arr->mat[k*arr->cols + j]);
            }
        }
    }
}

void print_mat(struct mat* arr){
    // to change the width, change in printf statement, and make int 3 lines down +1
    int i,j;
    printf("┌");
    for (int i = 0; i < 6*arr->cols; i++) {
        printf(" ");
    }
    printf("┐\n");

    for (i = 0; i < arr->rows; i++){
        printf("│");
        for (j = 0; j < arr->cols; j++){ 
            printf("%5.d ", arr->mat[i*arr->cols + j]);
        } 
            printf("│\n");
    }
    printf("└");
    for (int i = 0; i < 6*arr->cols; i++) {
        printf(" ");
    }
    printf("┘\n");
}

void fill_mat(struct mat* arr){
    int i, j, count = 0;
    for (i = 0; i < arr->rows; i++)
        for (j = 0; j < arr->cols; j++) 
         arr->mat[i*arr->cols + j] = ++count;
}

int sum_array(int* arr, int size){
    int sum = 0;
    for (int i = 0; i <size; i++) {
       sum += arr[i]; 
    }
    return sum;
}
