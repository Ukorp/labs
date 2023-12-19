#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    ok, memory_allocation_problem, wrong_int,
    wrong_double, wrong_argument_quantity,
    wrong_arguments, overflow, no_x

}errors;

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

long double max(long double a, long double b, long double eps){
    if ((a - b) > eps) return a;
    return b;
}

long double min(long double a, long double b, long double eps){
    if ((a - b) < eps) return a;
    return b;
}

char plus_minus(long double a){
    if (a < 0) return '-';
    else return '+';
}

char only_minus(long double a){
    if (a < 0) return '-';
    else return ' ';
}


enum flags{
    q, m, t, no_flag
};

enum flags define_flag(char * fl){
    if ((strcmp(fl, "-q") == 0) || (strcmp(fl, "/q") == 0)) return q;
    if ((strcmp(fl, "-m") == 0) || (strcmp(fl, "/m") == 0)) return m;
    if ((strcmp(fl, "-t") == 0) || (strcmp(fl, "/t") == 0)) return t;
    return no_flag;
}

int quadric(long double a, long double b, long double c, long double eps, long double * ans_array){
    long double D = b * b - 4 * a * c;
    if (D < 0) return no_x;
    else {
        ans_array[0] = (- 1 * b + sqrt(D)) / (2 * a);
        ans_array[1] = (- 1 * b - sqrt(D)) / (2 * a);
    }
    return ok;

}

void print_ans(long double a, long double b, long double c, long double * ans_array, long double eps){
    printf("%c %Lfx^2 %c %Lfx %c %Lf = 0\n", only_minus(a) ,(long double)fabs(a), plus_minus(b), (long double)fabs(b), plus_minus(c), (long double)fabs(c));
    if(quadric(a, b, c, eps, ans_array) == ok){
            
        if (fabs(ans_array[0] - ans_array[1]) < eps)
            printf("Корень уравнения x = %.3Lf\n\n", ans_array[0]);
        else printf("Корни уравнения x1 = %.3Lf, x2 = %.3Lf\n\n", ans_array[0], ans_array[1]);
    }

    else  printf("Данное квадратное уравнение не имеет рациональных корней.\n\n");
}

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

int main(int argc, char * argv[]){
    long double eps;

    long double * ans_array = (long double *)malloc(sizeof(long double) * 2);
    if (ans_array == NULL){
        free(ans_array);
        return memory_allocation_problem;
    }
    if (argc < 2) {
        printf("Ошибка\n");
        free(ans_array);
        return wrong_argument_quantity;
    }
    switch (define_flag(argv[1])){
        case q:
            if (argc != 6){
                printf("Указанно неверное количество аргументов.\n");
                free(ans_array);
                return wrong_argument_quantity;
            }
            if ((is_double(argv[2]) || is_double(argv[3]) || is_double(argv[4]) || is_double(argv[5])) != ok){
                printf("Неверно поданы аргументы\n");
                free(ans_array);
                return wrong_arguments;
            }
            long double a, b, c;
            char * stop1;
            char * stop2;
            char * stop3;
            char * stop4;
            eps = strtold(argv[2], &stop1);
            a = strtold(argv[3], &stop2);
            b = strtold(argv[4], &stop3);
            c = strtold(argv[5], &stop4);
            if (eps <= 0){
                printf("Эпсилон должен быть > 0\n");
                free(ans_array);
                return wrong_arguments;
            }
            if ((define_overflow_double(a*a) || define_overflow_double(b*b) || define_overflow_double(c*c)) != ok){
                printf("Переполнение\n");
                free(ans_array);
                return overflow;
            }
            print_ans(a, b, c, ans_array, eps);
            print_ans(a, c, b, ans_array, eps);
            print_ans(b, a, c, ans_array, eps);
            print_ans(b, c, a, ans_array, eps);
            print_ans(c, b, a, ans_array, eps);
            print_ans(c, a, b, ans_array, eps);
            break;
        
        case m:
            if (argc != 4){
                printf("Указанно неверное количество аргументов.\n");
                free(ans_array);
                return wrong_argument_quantity;
            }
            if ((is_int(argv[2]) && is_int(argv[3])) != ok){
                printf("Неверно поданы аргументы\n");
                free(ans_array);
                return wrong_int;
            }
            char * stop;
            long long first = strtoll(argv[2], &stop, 10);
            long long second = strtoll(argv[3], &stop, 10);
            if ((define_overflow_longlong(first) || define_overflow_longlong(second)) != ok){
                printf("Переполнение\n");
                free(ans_array);
                return overflow;
            }
            if (first % second == 0) printf("%lld кратно %lld\n", first, second);
            else printf("%lld не кратно %lld\n", first, second);
            break;

        case t:
            if (argc != 6){
                printf("Указанно неверное количество аргументов.\n");
                free(ans_array);
                return wrong_argument_quantity;
            }
            if ((is_double(argv[2]) || is_double(argv[3]) || is_double(argv[4]) || is_double(argv[5])) != ok){
                printf("Неверно поданы аргументы\n");
                free(ans_array);
                return wrong_arguments;
            }
            long double x, y, z, big, small, mid;
            eps = strtold(argv[2], &stop1);
            x = strtold(argv[3], &stop2);
            y = strtold(argv[4], &stop3);
            z = strtold(argv[5], &stop4);
            if (eps <= 0){
                printf("Эпсилон должен быть > 0\n");
                free(ans_array);
                wrong_arguments;
            }
            if ((define_overflow_double(eps) || define_overflow_longlong(x*x) || define_overflow_longlong(y*x) || define_overflow_longlong(z*z)) != ok) {
                printf("Переполнение\n");
                free(ans_array);
                return overflow;
            }
            big = max(x, max(y, z, eps), eps);
            small = min(x, min(y, z, eps), eps);
            mid = x + y + z - big - small;
            if (small <= eps){
                printf("Данные числа не могут являться сторонами прямоугольного треугольника\n");
            }
            else if (fabs(big * big - mid * mid - small * small) < eps) printf("Данные числа могут являться сторонами прямоугольного треугольника\n");
            else printf("Данные числа не могут являться сторонами прямоугольного треугольника\n");
            break;


        case no_flag:
            printf("Данного флага не существует\n");
            free(ans_array);
            return wrong_arguments;
    }
    free(ans_array);
    return ok;
}