#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum errors{
    ok, fail, memory_allocation_problem
}errors;

void peredel_kefof(int degree, double ** array){
    for (int i = 0; i < degree; i++){
        (*array)[i] = (*array)[i + 1] * (i + 1);
    }
    (*array)[degree] = 0;
}

double func(int degree, double x, double * array){
    double ans = 0;
    double start_x = x;
    for (int i = 0; i <= degree; i++){
        if (i == 0){
            ans += array[i];
            continue;
        }
        ans += array[i] * x;
        x = x * start_x;
    }
    
    return ans;
    
}

void dif_test(){
    // f(x) = x * x;
    double h = 1e-10;
    printf("%f\n", ((1 + h) * (1 + h) - 1 * 1) / h);
}

double diff(int degree, double x, double * array, double(* f) (int, double, double *))
{
    const double h = 1e-10;
    return (f(degree, x + h, array) - f(degree, x, array)) / h;
}

errors new_polynom(double eps, double a, double ** array, int degree, ...){
    *array = (double *)malloc(sizeof(double) * (degree + 1));
    double * coefficients = (double *)malloc(sizeof(double) * (degree + 1));
    if (*array == NULL || coefficients == NULL){
        return memory_allocation_problem;
    }
    int n = 1;
    double tmp;
    va_list list;
    va_start(list, degree);
    for (int i = 0; i <= degree; i++){
        tmp = va_arg(list, double);
        coefficients[i] = tmp;
    }
    
    for (int i = 0; i <= degree; i++){
        if (i != 0) n *= i;
        (*array)[i] = func(degree - i, a, coefficients) / n;
        
        peredel_kefof(degree-i, &coefficients);
    }
    va_end(list);
    free(coefficients);
    return ok;
}

int main()
{
    double * array;
    new_polynom(0.000000001, 3, &array, 2, 1.0, 2.0, 1.0);
    printf("%f %f %f\n", array[0], array[1], array[2]);
    free(array);
    return 0;
}
