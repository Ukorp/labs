#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int bin_search(int * a, int size, int num){
    int mid = -1;
    int l = 0;
    int r = size;
    while (mid != (r + l) / 2){
        mid = (r + l) / 2;
        if (a[mid] == num) return num;
        if (a[mid] < num) l = mid;
        if (a[mid] > num) r = mid;
    }
    return a[mid];

}

int cmp(const int *, const int *);

int main(){
    int size1, size2;
    unsigned int stime = (unsigned int)time(NULL) / 2;
    srand(stime);
    size1 = (10 + rand() % (40 - 10 + 1));
    size2 = (10 + rand() % (40 - 10 + 1));
    int * A = (int *)malloc(sizeof(int) * size1);
    int * B = (int *)malloc(sizeof(int) * size2);
    int * C = (int *)malloc(sizeof(int) * size1);
    if (A == NULL && B == NULL && C == NULL){
        printf("Ошибка выделения памяти\n");
        free(A);
        free(B);
        free(C);
        return 0;
    }
    for (int i = 0; i < size1; ++i)
        A[i] = (-1000 + rand() % (2000 + 1));
    for (int i = 0; i < size2; ++i)
        B[i] = (-1000 + rand() % (2000 + 1));        

    qsort(B, (size_t)size2, sizeof(int), (int (*)(const void *, const void *))cmp);

    for (int i = 0; i < size1; i++){
        C[i] = A[i] + bin_search(B, size2, A[i]);
    }

    for (int i = 0; i < size1; i++){
        printf("%d ", A[i]);
    }
    printf("\n============================================================\n");
    for (int i = 0; i < size2; i++){
        printf("%d ", B[i]);
    }
    printf("\n============================================================\n");
    for (int i = 0; i < size1; i++){
        printf("%d ", C[i]);
    }
    

    free(A);
    free(B);
    free(C);
}

int cmp(const int * a, const int * b){
    return *a - *b;
}