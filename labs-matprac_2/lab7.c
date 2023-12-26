#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>



typedef enum errors{
    ok, not_ok, no_flag, argument_error,
    unknown_error, wrong_int, overflow,
    memory_allocation_problem, wrong_interval
}errors;

errors define_overflow_longlong(long long a){

    if ((a >= LLONG_MAX) || (a <= LLONG_MIN)) {
        return overflow;
    }
    else return ok;
}

double equation(double x){
    return 0.3 * x * x - 3 * x;
}

double dichotomy_method(errors * status_code, double l, double r, double eps, double (* equation)(double)){
    if (equation(l) * equation(r) > 0) return wrong_interval;
    if (r - l < eps){
        *status_code = wrong_interval;
        return wrong_interval;
    }
    double mid;
    while (r - l > eps){
        mid = (r + l)/2;
        if (equation(mid) * equation(l) > 0) l = mid;
        else r = mid;
    }
    if (equation((r + l)/2) > eps){
        *status_code = wrong_interval;
        return wrong_interval;
    }
    *status_code = ok;
    return (r + l) / 2;
}



int main(){
    double ans;
    errors status_code = ok;
    ans = dichotomy_method(&status_code,5, 15, 0.00000001, equation);
    if (status_code == wrong_interval){
        printf("Неверный интервал\n");
        return wrong_interval;
    }
    if (define_overflow_longlong((long long)ans) == overflow){
        puts("overflow\n");
        return overflow;
    }
    printf("%f\n", ans);
}