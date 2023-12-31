#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define STR_SIZE 256

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;


typedef struct Vector {
    double* coordinates;
} Vector;

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}

void print_vector(Vector* arr, int size) {
    printf("<");
    for (int i = 0; i < size; i++) {
        printf("%.3lf", arr->coordinates[i]);
        if (i != size - 1) {
            printf(", ");
        }
    }
    printf(">");
    printf("\n");
}

status_code binary_pow(double* res, double value, int degree) {
    const double EPS = 1e-8;
    if (degree == 0) {
        *res = 1.0;
        return code_success;
    }
    if (fabs(value) < EPS && abs(degree) < EPS) {
        return code_invalid_parameter;
    }
    double tmp;
    if (degree > 0) {
        status_code tmp_status = binary_pow(&tmp, value, degree / 2);

        if ((degree & 1) == 0) {
            *res = tmp * tmp;
        } else {
            *res = value * tmp * tmp;
        }
    } else {
        status_code tmp_status = binary_pow(&tmp, value, -degree);
        if (tmp_status != code_success) {
            return tmp_status;
        }
        *res = 1.0 / tmp;
    }   
    if (isinf(*res)) {
        return code_invalid_parameter;
    }
    if (isnan(*res)) {
        return code_invalid_parameter;
    }
    return code_success;
}

status_code get_golder_norm(double* res, Vector* vect, int n, int p) {
    *res = 0.0;
    if (p < 1) {
        return code_invalid_parameter;
    }
    for (int i = 0; i < n; i++) {
        double tmp_res;
        status_code st_pow = binary_pow(&tmp_res, fabs(vect->coordinates[i]), p);
        if (st_pow != code_invalid_parameter) {
            *res += tmp_res;
        } else {
            return code_invalid_parameter;
        }
    }
    return code_success;
}

status_code get_golder_inf(double* res, Vector* vect, int n, int p) {
    *res = 0.0;
    for (int i = 0; i < n; i++) {
        *res = fmax(*res, fabs(vect->coordinates[i]));
    }
    return code_success;
}

status_code mult_matr_vect(double** a, double* x, double** res, int size_m,  int size_v) {
    if (size_m != size_v) {
        return code_invalid_parameter;
    }
    (*res) = malloc(sizeof(double) * size_v);
    if (*res == NULL) {
        return code_error_alloc;
    }
    for (int i = 0; i < size_m; i++) {
        (*res)[i] = 0.0;
        for (int j = 0; j < size_m; j++) {
            (*res)[i] += a[i][j] * x[j];
        }
    }
    return code_success;
}

status_code get_matrix_norm(double* res, Vector* vect, int n, int p) {
    double* res_mult = NULL;
    double** A = (double**)malloc(sizeof(double*) * n);
    if (!A) {
        return code_error_alloc;
    }
    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(sizeof(double) * n);
        if (!A[i]) {
            for (int j = 0; j < i; j++) {
                free(A[j]);
            }
            free(A);
            return code_error_alloc;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == i) {
                A[i][j] = 1;
            } else {
                A[i][j] = 0;
            }
        }
    }
    status_code st_mult = mult_matr_vect(A, vect->coordinates, &res_mult, n, n);
    if (st_mult == code_error_alloc) {
        return code_error_alloc;
    }
    *res = 0.0;
    for (int i = 0; i < n; i++) {
        *res += res_mult[i] * vect->coordinates[i];
    }
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(res_mult);
    return code_success;
}

status_code get_max_len_vector(Vector** max_vectors, int* size, status_code norm(double* res, Vector* vect, int n, int p), int dimension, int p, int count,...) {
    if (count < 0 || dimension < 1) {
        return code_invalid_parameter;
    }
    *size = count;
    const double epsilon = 1e-10;
    va_list ptr;
    (*max_vectors) = (Vector*)malloc(sizeof(Vector) * count);
    if (!(*max_vectors)) {
        return code_error_alloc;
    }
    va_start(ptr, count);
    Vector first = va_arg(ptr, Vector);
    //print_vector(&first, dimension);
    double max = 0.0;
    status_code st_norm = norm(&max, &first, dimension, p);
    if (st_norm == code_error_alloc) {
        return code_error_alloc;
    }
    int index_max = 0;
    for (int i = 1; i < count; i++) {
        Vector  tmp = va_arg(ptr, Vector);
        //print_vector(&tmp, dimension);
        double len_tmp;
        st_norm = norm(&len_tmp, &tmp, dimension, p);
        if (st_norm == code_error_alloc) {
            return code_error_alloc;
        }
        if (len_tmp - max > epsilon) {
            max = len_tmp;
            index_max = 1;
            (*max_vectors)[0] = tmp; 
        } else if (fabs(len_tmp - max) < epsilon) {
            (*max_vectors)[index_max++] = tmp; 
        }
    }
    *size = index_max;
    va_end(ptr);
    return code_success;
}

int main(int argc, char* argv[]) {
    Vector* maxs = NULL;
    Vector vector1, vector2, vector3, vector4, vector5;
    int dimension = 3;
    vector1.coordinates = (double*)malloc(dimension * sizeof(double));
    vector1.coordinates[0] = 1.0;
    vector1.coordinates[1] = 2.0;
    vector1.coordinates[2] = 3.0;

    vector2.coordinates = (double*)malloc(dimension * sizeof(double));
    vector2.coordinates[0] = 24.0;
    vector2.coordinates[1] = 0.0;
    vector2.coordinates[2] = 1.0;

    vector3.coordinates = (double*)malloc(dimension * sizeof(double));
    vector3.coordinates[0] = 0.0;
    vector3.coordinates[1] = 0.0;
    vector3.coordinates[2] = 0.0;

    vector4.coordinates = (double*)malloc(dimension * sizeof(double));
    vector4.coordinates[0] = 24.0;
    vector4.coordinates[1] = 3.0;
    vector4.coordinates[2] = 3.0;

    vector5.coordinates = (double*)malloc(dimension * sizeof(double));
    vector5.coordinates[0] = 15.0;
    vector5.coordinates[1] = -7.0;
    vector5.coordinates[2] = 23.0;
    int count = 5;
    int size;
    int p = 1;
    status_code st_find = get_max_len_vector(&maxs, &size, get_matrix_norm, dimension, p, count, vector1, vector2, vector3, vector4, vector5);
    if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free_all(6, vector1.coordinates, vector2.coordinates, vector3.coordinates, vector4.coordinates, vector5.coordinates, maxs);
        return -2;
    }
    for (int i = 0; i < size; i++) {
        Vector tmp = maxs[i];
        print_vector(&tmp, dimension);
    }
    free_all(6, vector1.coordinates, vector2.coordinates, vector3.coordinates, vector4.coordinates, vector5.coordinates, maxs);
    return 0;

}