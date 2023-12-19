#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

long long fact(int x){
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

double find_e1(double eps){
    double e1 = 0;
    int n = 1;
    double e2 = pow((1 + (1.0 / n)), n);
    while (fabs(e2-e1) > eps){
        n++;
        e1 = e2;
        e2 = pow((1 + (1.0 / n)), n);
    }
    printf("%f\n", e2);
}

double find_e2(double eps){
    double e, e1;
    e = 2;
    int n = 2;
    do {
        e1 = 1.0 / (fact(n));
        e += e1;
        n++;
    } while (fabs(e1 - (1.0 / fact(n))) > eps);
    return e;
}

double find_e3(double eps){
    double l = 0;
    double r = 10;
    double mid;
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
    printf("%f\n", l);
}

double find_pi2(double eps){
    double pi, pi1;
    pi = 0;
    int n = 1;
    do {
        pi1 = pow(-1, n-1) / (2*n - 1);
        // printf("%le\n", pi1);
        pi += pi1;
        n++;
        if (n > 1000000) break;
    } while (fabs(pow(-1, n-1) / (2*n - 1)) > eps);
    return 4 * pi;
}

long double find_pi1(double eps){
    long double pi1 = 0;
    long int n = 1;
    long double pi2 = 1.0 * pw(pw(2, n) * fact(n), 4) / (1.0 * n * pw(fact(2*n), 2));
    while (fabs(pi2-pi1) > eps){
        n++;
        pi1 = pi2;
        if (n > 1000000) break;
        pi2 = 1.0 * pow(pow(2, n) * fact(n), 4) / (1.0 * n * pow(fact(2*n), 2));
        // printf("%f\n", pi2);
    }
    printf("%Le\n", pi2);
}

// double find_pi3(double eps){
//     double l = 3;
//     double r = 4;
//     double mid;
//     while (fabs(r - l) > eps){
//         mid = (r + l) / 2;
//         if (((sin(mid)) - 1) > eps){
//             l = mid;
            
//         }
//         else if (((sin(mid)) - 1) < eps){
//             r = mid;
//             if (fabs((sin(mid)) - 1) < eps) break;
            
//         }
//     }
//     printf("%f\n", mid);
// }

double find_ln1(double eps){
    double ln1 = 0;
    int n = 1;
    double ln2 = n * (pow(2, 1.0/n) - 1);
    while (fabs(ln2-ln1) > eps){
        n++;
        ln1 = ln2;
        ln2 = n * (pow(2, 1.0/n) - 1);
    }
    printf("%f\n", ln2);
}

// double find_ln2(double eps){
//     double ln, ln1;
//     ln = 1;
//     int n = 2;
//     do {
//         ln1 = pow(-1.0, n-1) / n;
//         // printf("%f\n", ln);
//         ln += ln1;
//         n++;
//         if (n > 1000) break;
//     } while (fabs(fabs(ln1) - fabs(pow(-1.0, n-1) / n)) > eps);
//     return ln;
// }

double find_ln3(double eps){
    double l = 0;
    double r = 10;
    double mid;
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
    printf("%f\n", l);
}

double find_sqrt1(double eps){
    double sqrt1 = - 0.5; 
    double sqrt = sqrt1;
    int n = 1;
    double sqrt2 = sqrt1 - pow(sqrt1, 2) / 2 + 1;
    while (fabs(sqrt2-sqrt) > eps){
        n++;
        sqrt = sqrt1;
        sqrt2 = sqrt1 - pow(sqrt1, 2) / 2 + 1;
        sqrt1 = sqrt2;

        if (n > 1000000) break;
    }
    printf("%f\n", sqrt2);
}

double find_ln2(long double eps){
    double ln, ln1;
    ln = 1;
    int n = 2;
    int z = 1;
    do {
        z *= -1;
        ln1 = z * 1.0 / n;
        ln += ln1;
        n++;
        if (n > 1000000) break;
    } while (fabs(fabs(ln1) - (1.0 / n)) > eps);
    printf("--|%f|----|%f|----%d--\n", fabs(ln1) - 1.0 / n, eps, (fabs(ln1) - 1.0 / n) > eps);
    return ln;
}

double find_sqrt2(double eps){
    double sqrt, sqrt1;
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

double find_sqrt3(double eps){
    double l = 1;
    double r = 2;
    int n = 0;
    double mid;
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
    printf("%f\n", mid);
}

int main(int argc, double * argv[]){
    if (argc != 2) {
        printf("Ошибка\n");
        return 0;
    }
    // printf("--%d--%d--%d--\n", 1 << 4, pw(2, 4), pw(3, 3));
    // printf("%f\n", find_e(*argv[1]));
    printf("%f\n", find_pi2(*argv[1]));

    find_e1(*argv[1]);
    find_e3(*argv[1]);
    find_ln1(*argv[1]);
    find_pi1(*argv[1]);

    printf("%f\n", find_ln2(*argv[1]));
    find_ln3(*argv[1]);
    // find_pi3(*argv[1]);
    find_sqrt1(*argv[1]);
    printf("%f\n", find_sqrt2(*argv[1]));
    find_sqrt3(*argv[1]);
}
    