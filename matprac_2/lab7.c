#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum errors{
    ok, wrong_interval
}errors;

double equation(double x){
    return 0.3 * x * x - 3 * x;
}

double dichotomy_method(double l, double r, double eps, double (* equation)(double)){
    if (equation(l) * equation(r) > 0) return wrong_interval;
    if (r - l < eps) return wrong_interval;
    double mid;
    while (r - l > eps){
        mid = (r + l)/2;
        if (equation(mid) * equation(l) > 0) l = mid;
        else r = mid;
    }
    if (equation((r + l)/2) > eps) return wrong_interval;
    return (r + l) / 2;
}

int main(){
    double ans;
    if ((ans = dichotomy_method(5, 15, 0.00000001, equation)) == wrong_interval){
        printf("Неверный интервал\n");
        return wrong_interval;
    }
    printf("%f\n", ans);
}