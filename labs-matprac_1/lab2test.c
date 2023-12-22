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

long long fact(long int x){
    long long ans = x;
    if (x == 0) return 1;
    if (x <= 2) return x;
    else {
        while (x != 1){
            ans = ans * (x-1);
            x--;
        }
    }
    return ans;
}


int pw (int x, int n){
    int x0 = x;
    for (int i = 1; i < n; i++) x *= x0;
    return x;
}

int is_double(char * str){
    int n = 0;
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '.') && (n == 0) && (i != 0) && (i != (strlen(str) - 1))) n++;
        else if (str[i] < '0' || str[i] > '9') 
            return 0;
    }
    return 1;
}

long double find_e1(long double eps){
    long double e1 = 0;
    int n = 1;
    long double e2 = pow((1 + (1.0 / n)), n);
    while (fabs(e2-e1) > eps){
        n++;
        e1 = e2;
        e2 = pow((1 + (1.0 / n)), n);
    }
    return e2;
}

long double find_e2(long double eps){
    long double e, e1;
    e = 2;
    int n = 2;
    do {
        e1 = 1.0 / (fact(n));
        e += e1;
        n++;
    } while (fabs(e1 - (1.0 / fact(n))) > eps);
    return e;
}

long double find_e3(long double eps){
    long double l = 0;
    long double r = 10;
    long double mid;
    while (fabs(r - l) > eps){
        mid = (r + l) / 2;
        if (((log(mid)) - 1.0) > eps){
            r = mid;
            
        }
        else if ((log(mid) - 1.0) < eps){
            l = mid;
            if (fabs(log(mid) - 1.0) < eps) break;
            
        }
    }
    return l;
}

long double find_pi2(long double eps){
    long double pi, pi1;
    int n = 1;
    int z = 1;
    pi = (long double)(z * 1.0 / (2*n - 1));
    n++;
    do {
        z *= -1;
        pi1 = (long double)(z * 1.0 / (2*n - 1));
        pi += pi1;
        n++;
        // if (n > 1000000) break;
    } while (fabs(fabs(pi1) - (long double)(1.0 / (2*n - 1))) > eps);
    return 4 * pi;
}

long double find_pi1(long double eps){
    long double pi1 = 0;
    long double pi2 = 1;
    long int n = 200;
    for (long int i = (n+1); i <= 2*n; i++){
        pi2 *= (long double)(i-n)/ i * (i-n) / i;
    }
    pi2 *= (long double)pow(2, 4 * n) / n;
    while (fabs(pi2-pi1) > eps){
        n++;
        pi1 = pi2;
        pi2 = 1;
        
        for (int i = (n+1); i <= 2*n; i++){ 
            pi2 *= (i-n);
            pi2 /= i; 
            pi2 *= (i-n);
            pi2 /= i; 
            pi2 *= 16;
        }
        pi2 /= n;
    }
    return pi2;
}

long double find_pi3(long double eps){
    long double l = 3;
    long double r = 4;
    long double mid;
    while (fabs(r - l) > eps){
        mid = (r + l) / 2;
        if ((sin(mid)) > eps){
            l = mid;
            
        }
        else if ((sin(mid)) < eps){
            r = mid;
            if (fabs((sin(mid)) - 1) < eps) break;
            
        }
    }
    return mid;
}

long double find_ln1(long double eps){
    long double ln1 = 0;
    int n = 1;
    long double ln2 = n * (pow(2, 1.0/n) - 1);
    while (fabs(ln2-ln1) > eps){
        n++;
        ln1 = ln2;
        ln2 = n * (pow(2, 1.0/n) - 1);
    }
    return ln2;
}

long double find_ln3(long double eps){
    long double l = 0;
    long double r = 10;
    long double mid;
    while (fabs(r - l) > eps){
        mid = (r + l) / 2;
        if (((exp(mid)) - 2.0) > eps){
            r = mid;
            
        }
        else if ((exp(mid) - 2.0) < eps){
            l = mid;
            if (fabs(exp(mid) - 2.0) < eps) break;
            
        }
    }
    return l;
}

long double find_sqrt1(long double eps){
    long double sqrt1 = - 0.5; 
    long double sqrt = sqrt1;
    int n = 1;
    long double sqrt2 = sqrt1 - pow(sqrt1, 2) / 2 + 1;
    while (fabs(sqrt2-sqrt) > eps){
        n++;
        sqrt = sqrt1;
        sqrt2 = sqrt1 - pow(sqrt1, 2) / 2 + 1;
        sqrt1 = sqrt2;

    }
   return sqrt2;
}

long double find_ln2(long double eps){
    long double ln, ln1;
    ln = 1;
    int n = 2;
    int z = 1;
    do {
        z *= -1;
        ln1 = z * 1.0 / n;
        ln += ln1;
        n++;
    } while (fabs(fabs(ln1) - (1.0 / n)) > eps);
    return ln;
}

long double find_sqrt2(long double eps){
    long double sqrt, sqrt1;
    int k = 2;
    sqrt = pow(2, pow(2, -k));
    k++;
    do {
        sqrt1 = pow(2, pow(2, -k));
        sqrt *= sqrt1;
        k++;
    } while (fabs(fabs(sqrt1) - pow(2, pow(2, -k))) > eps);
    return sqrt;
    
}

long double find_sqrt3(long double eps){
    long double l = 1;
    long double r = 2;
    int n = 0;
    long double mid;
    while ((fabs(r - l) > eps) && (n < 100000)){
        mid = (r + l) / 2;
        if ((pow(mid, 2) - 2) > eps){
            r = mid;
            
        }
        else if ((pow(mid, 2) - 2) < eps){
            l = mid;
            if (fabs(pow(mid, 2) - 2) < eps)break;
            
        }
        n++;
    }
    return mid;
}

long double find_y1(long double eps){
    long double y1 = 0;
    long double y = 0;
    long long n = 1;
    long double y2 = 0;
    y2 = 1.0 / n;
    y += y2;
    do{
        n++;
        y1 = y2;
        y2 = 1.0 / n;
        y += y2;

    }while (fabs(fabs(y2)-fabs(y1)) > eps);
    y -= logl(n);
    return y;
}

long double find_y2(long double eps){
    long double y1, y2, y, pi, k;
    y1 = 0;
    y = 0;
    k = 2;
    pi = find_pi2(eps);
    y = - pi * pi / 6;
    y2 = 1.0 / ((int)sqrt(k) * (int)sqrt(k)) - 1.0 / k;
    y += y2;
    while (fabs(y1 - y2) > eps){
        k++;
        y1 = y2;
        y2 = 1.0 / ((int)sqrt(k) * (int)sqrt(k)) - 1.0 / k;
        y += y2;
    }
    return y;

}

int is_simple(long int num){
    if (num == 1) return 0;
    if (num == 2) return 1;
    for (long int i = 2; i < sqrt(num); i++){
        if (num % i == 0) return 0;
    }
    return 1;
}

long double count_y3(long double eps, long int * p){
    long double multi2, multi1, multi;
    multi = 1;
    multi1 = 0;
    multi2 = (long double)(*p-1)/(*p);
    multi *= multi2;
    while (fabs(multi1 - multi2) > eps){
        (*p)++;
        while (is_simple(*p) == 0) (*p)++;
        multi1 = multi2;
        multi2 = (long double)((*p)-1)/(*p);
        multi *= multi2;
        
    }
    return multi;
}

long double find_y3(long double eps){
    // long int * p = (long int *)malloc(sizeof(long int));
    long int p = 2;
    long double y1, y2;
    y1 = 0;
    y2 = 1;
    long double y = count_y3(eps, &p);
    long int t = p;
    while(fabs(y2 - y1) > eps){
        t++;
        y1 = y2;
        y2 = logl(t) * y; 
    }
    return y2;
    
}

int main(int argc, char * argv[]){
    if (argc != 2) {
        printf("Ошибка\n");
        return wrong_argument_quantity;
    }
    if (!is_double(argv[1])){
        printf("Неверно подан аргумент\n");
        return wrong_arguments;
    }
    char * stop;
    long double eps = strtold(argv[1], &stop);
    if (eps <= 0){
        printf("Неправильно подан аргумент\n");
        return wrong_arguments;
    }
    if (eps < 0.0000001){
        printf("Слишком маленький eps\n");
        return wrong_double;
    }

    printf("e = %Lf\n", find_e1(eps));
    printf("e = %Lf\n", find_e2(eps));
    printf("e = %Lf\n", find_e3(eps));

    printf("pi = %Lf\n", find_pi1(eps));
    printf("pi = %Lf\n", find_pi2(eps));
    printf("pi = %Lf\n", find_pi3(eps));
    


    printf("ln(2) = %Lf\n", find_ln1(eps));
    printf("ln(2) = %Lf\n", find_ln2(eps));
    printf("ln(2) = %Lf\n", find_ln3(eps));

    printf("sqrt(2) = %Lf\n", find_sqrt1(eps));
    printf("sqrt(2) = %Lf\n", find_sqrt2(eps));
    printf("sqrt(2) = %Lf\n", find_sqrt3(eps));

    printf("y = %Lf\n", find_y1(eps));
    printf("y = %Lf\n", find_y2(eps));
    printf("y = %Lf\n", -logl(find_y3(eps)));
}
    