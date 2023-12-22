#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    ok, memory_allocation_problem, wrong_int,
    wrong_double, wrong_argument_quantity,
    wrong_arguments, overflow, no_x,
    file_error

}errors;

errors define_overflow_double(long double a){

    if ((a >= LLONG_MAX) || (a <= LLONG_MIN)) {
        return overflow;
    }
    else return ok;
}

errors define_overflow_longlong(long long a){

    if ((a >= LLONG_MAX) || (a <= LLONG_MIN)) {
        return overflow;
    }
    else return ok;
}

int is_double(char * str){
    int n = 0;
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if ((str[i] == '.') && (n == 0) && (i != 0) && (i != (strlen(str) - 1))) n++;
        else if (str[i] < '0' || str[i] > '9') 
            return wrong_double;
    }
    return ok;
}

int is_int(char * str){
    
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return wrong_int;
    }
    return ok;
}

long double func_a(long double x){
    return logl(x + 1) / x;
}

long double func_b(long double x){
    return expl(-x * x / 2);
}

long double func_c(long double x){
    return logl(1 / (1-x));
}

long double func_d(long double x){
    return powl(x, x);
}

long double integral(long double (*f)(long double), long double eps){
    long double n = 1.0 / eps;
    long double ans = f(eps) + f(1-eps);
    for (int i = 1; i < n; ++i){
        ans += 2 * f(i * eps);
    }
    ans *= eps / 2;
    return ans;
}

int main(int argc, char * argv[]){
    if (argc != 2){
        printf("Неправильное количество аргументов\n");
        return wrong_argument_quantity;
    }
    if (is_double(argv[1]) != ok){
        printf("Неправильно поданы аргументы\n");
        return wrong_arguments;
    }
    char * stop;
    long double eps = strtold(argv[1], &stop);
    if (define_overflow_double(eps) != ok){
        printf("Переполнение\n");
        return overflow;
    }
    if (eps <= 0){
        printf("Эпсилон должен быть > 0\n");
        return wrong_arguments;
    }
    if (eps < 0.0000001){
        printf("Слишком маленький eps\n");
        return wrong_arguments;
    }
    printf("%Lf\n", integral(func_a, eps));
    printf("%Lf\n", integral(func_b, eps));
    printf("%Lf\n", integral(func_c, eps));
    printf("%Lf\n", integral(func_d, eps));
    return ok;
}