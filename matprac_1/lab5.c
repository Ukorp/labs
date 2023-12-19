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

long double find_a(long double eps, long double x){
    long long n = 0;
    long double sum1, sum2, sum;
    sum = 1;
    sum2 = 0;
    sum1 = 1;
    while (sum1 > eps){
        n++;
        
        sum += sum1 * x / n;
        
        sum2 = sum1;
        sum1 = sum1 * x / n;
    }
    return sum; 
}

long double find_b(long double eps, long double x){
    long long n = 0;
    long double sum1, sum2, sum;
    sum = 1;
    sum1 = 1;
    int z = 1;
    while (fabs(sum1) > eps){
        n++;
        z *= -1;
        sum1 = z * fabs(sum1) * x * x;
        sum1 /= (2 * n);
        sum1 /= (2 * n - 1);
        sum += sum1;
    }
    return sum; 
}

long double find_c(long double eps, long double x){
    long long n = 0;
    long double sum1, sum2, sum;
    sum = 1;
    sum1 = 1;
    while (fabs(sum1) > eps){
        n++;
        sum1 = sum1 * x * x;
        sum1 /= (3 * n);
        sum1 /= (3 * n - 1);
        sum1 /= (3 * n - 2);
        sum1 *= n * n * n ;
        sum1 /= 27;
        sum += sum1;
    }
    return sum; 
}

long double find_d(long double eps, long double x){
    long long n = 1;
    long double sum1, sum2, sum;
    int z = -1;
    sum1 = z * 0.5 * x * x;
    sum += sum1;
    while (fabs(sum1) > eps){
        n++;
        z *= -1;
        sum1 = z * x * x;
        sum1 /= (2 * n);
        sum1 *= (2 * n - 1);
        sum1 /= (2 * n - 2);
        sum1 *= (2 * n - 3);
        sum1 *= fabs(sum1);
        sum += sum1;
        if (n == 10000) break;
    }
    return sum; 
}

int main(int argc, char * argv[]){
    if (argc != 3){
        printf("Неверное количество аргументов\n");
        return wrong_argument_quantity;
    }
    if ((is_double(argv[1]) || is_double(argv[2])) != ok){
        printf("Неверно поданы аргументы\n");
        return wrong_arguments;
    }
    char * stop;
    long double eps = strtold(argv[1], &stop);
    long double x = strtold(argv[2], &stop);

    if ((define_overflow_double(eps) || define_overflow_double(x)) != ok){
        printf("Переполнение\n");
        return overflow;
    }
    if (x <= 0){
        printf("Число х должен быть > 0\n");
        return wrong_arguments;
    }
    if (eps <= 0){
        printf("Эпсилон должен быть > 0\n");
        return wrong_arguments;
    }

    printf("%Lf\n", find_a(eps, x));
    printf("%Lf\n", find_b(eps, x));
    printf("%Lf\n", find_c(eps, x));
    printf("%Lf\n", find_d(eps, x));
    return ok;
}