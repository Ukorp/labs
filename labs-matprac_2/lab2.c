#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>

typedef enum errors{
    ok, not_ok, no_flag, argument_error,
    unknown_error, wrong_int, overflow,
    memory_allocation_problem
}errors;

double geometric_mean(double eps, errors * errors_detection, int num, ...){
    double ans = 1;
    va_list ptr;
    va_start(ptr, num);
    double tmp = num;
    for(int i = 0; i < num; i++){
        tmp = va_arg(ptr, double);
        ans *= tmp;
        if (fabs(ans - DBL_MAX) < eps){
            *errors_detection = overflow;
            return not_ok;
        }
    }
    va_end(ptr);
    *errors_detection = ok;
    return pow(ans, 1.0 / num);
}

double binpow (double a, int n) {
	if (n == 0)
		return 1;
	if (n % 2 == 1)
		return binpow (a, n-1) * a;
	else {
		int b = binpow (a, n/2);
		return b * b;
	}
}

int main(){
    double eps = 0.000000001;
    errors errors_detection;
    double ans = geometric_mean(eps, &errors_detection, 3, 4.0, 8.0, 16.0);
    switch(errors_detection){
        case overflow:
            printf("Переполнение\n");
            return overflow;
        case argument_error:
            printf("Неверное количество аргументов\n");
            return argument_error;
        case ok:
            printf("%f\n", ans);
            printf("%f\n", binpow(2, 4));
            return ok;
        default:
            printf("Неизвестная ошибка\n");
            return unknown_error;
    }
    
    
}