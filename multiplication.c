#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct mat {
    int rows;
    int cols;
    int *ptr;
};

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
            printf("%5.d ", arr->ptr[i*arr->cols + j]);
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
         arr->ptr[i*arr->cols + j] = ++count;
}

void make_mat(int rows, int cols, struct mat* arr){
    arr->ptr = (int *)malloc(rows * cols * sizeof(int));
    arr->rows = rows;
    arr->cols = cols;
}


void multiply(struct mat* arr, struct mat* arr2, struct mat* result ){
    int i,j,k;
    
    for (i=0;i<arr->rows;i++){
        for (j=0;j<arr2->cols;j++){
            for (k=0;k<arr->cols;k++){
                result->ptr[i*arr->cols + j] += 
                    (arr->ptr[i*arr->cols + k] * arr->ptr[k*arr->cols + j]);
            }
        }
    }
}

int main()
{
    struct mat arr;
    struct mat arr2;
    struct mat arr_result;

    int num=750;

    make_mat(num,num,&arr);
    make_mat(num,num,&arr2);
    make_mat(num,num,&arr_result);

    fill_mat(&arr); 
    fill_mat(&arr2); 


    multiply(&arr, &arr2, &arr_result);


    free(arr.ptr);
    free(arr2.ptr);
    free(arr_result.ptr);

    return 0;
}
