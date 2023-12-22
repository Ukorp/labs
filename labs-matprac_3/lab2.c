#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum errors{
    ok, fail, memory_allocation_problem
}errors;

typedef struct Vector {
    double * data;
    int size;
} Vector;

typedef struct Matrix{
    int size;
    double ** data;
} Matrix;

Vector * create_Vector(int size){
    Vector * vec = (Vector *)malloc(sizeof(Vector));
    vec->size = size;
    vec->data = (double *)malloc(sizeof(double) * size);
    if (vec->data == NULL) return NULL;
    return vec;
}

Matrix * create_Matrix(int size){
    Matrix * mat = (Matrix *)malloc(sizeof(Matrix));
    mat->size = size;
    mat->data = (double **)malloc(sizeof(double *) * size);
    if (mat->data == NULL) return NULL;
    for (int i = 0; i < size; ++i){
        mat->data[i] = (double *)malloc(sizeof(double) * size);
        if (mat->data[i] == NULL) return NULL;
    }
    return mat;
}

void freeVector(Vector * vector){
    free(vector->data);
    free(vector);
}

void freeMatrix(Matrix * mat){
    for (int i = 0; i < mat->size; i++){
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

double norm_x_p(Vector * vec, int p){
    double ans = 0;
    for (int i = 0; i < vec->size; i++){
        ans += pow(vec->data[i], p);
    }
    return pow(ans, 1.0 / p);
}

double norm_x_inf(Vector * vec){
    double ans = vec->data[0];
    for (int i = 0; i < vec->size; i++){
        if (ans < vec->data[i])
            ans = vec->data[i];
    }
    return ans;
}

double norm_x_A(Vector * x, Matrix * mat){
    if (x->size != mat->size) return 0;
    double ans = 0;
    for (int i = 0; i < x->size; i++){
        double num = 0;
        for (int j = 0; j < x->size; j++){
            num += mat->data[i][j] * x->data[j];
        }
        ans += num * x->data[i];
    }
    return sqrt(ans);

}

int main() {
    Vector * vec = create_Vector(3);
    Matrix * mat = create_Matrix(3);
    if (vec == NULL || mat == NULL){
        puts("Memory allocation problem\n");
        return memory_allocation_problem;
    }
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            mat->data[i][j] = i+j;
        }
    }
    vec->data[0] = 1;
    vec->data[1] = 3;
    vec->data[2] = 24;
    printf("%f ", norm_x_p(vec, 7));
    printf("%f ", norm_x_inf(vec));
    printf("%f ", norm_x_A(vec, mat));
    freeVector(vec);
    freeMatrix(mat);
}
