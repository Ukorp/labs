#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

long long is_number(char * str){
    
    for (long long i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return 0;
    }
    return 1;
}

long long what_number(char * str){
    if ((strcmp(str, "-h") == 0) || (strcmp(str, "/h") == 0)) return 1;
    if ((strcmp(str, "-p") == 0) || (strcmp(str, "/p") == 0)) return 2;
    if ((strcmp(str, "-s") == 0) || (strcmp(str, "/s") == 0)) return 3;
    if ((strcmp(str, "-e") == 0) || (strcmp(str, "/e") == 0)) return 4;
    if ((strcmp(str, "-a") == 0) || (strcmp(str, "/a") == 0)) return 5;
    if ((strcmp(str, "-f") == 0) || (strcmp(str, "/f") == 0)) return 6;
    return 0;
}

long long fact(long long x){
    long long ans = x;
    if (x > 20) return -1;
    if (x <= 2) return x;
    else {
        while (x != 1){
            ans = ans * (x-1);
            x--;
        }
    }
    return ans;
}

long long size_int(long long x){
    int i = 0;
    while (x != 0){
        i++;
        x /= 10;
    }
    return i;
}

int main(long long argc, char * argv[]){
    if (!is_number(argv[1])){
        printf("НЕПРАВИЛЬНО ЗАДАНО ЧИСЛО\n");
        return 0;
    }

    if (argc != 3) {
        printf("Ошибка\n");
        return 0;
    }
    long long num = atoll(argv[1]);
    long long k = what_number(argv[2]);
    if (LLONG_MAX <= num){
        printf("Переполнение\n");
        return 0;
    }
    switch (k){
        case 1:
            num = fabs(num);
            long long num0 = num;
            while (num <= 100){
                printf("%lld ", num);
                num += num0;
            }
            printf("\n");
            break;
        case 2:
            num = fabs(num);
            if (num < 2){
                printf("NO\n");
                break;
            }
            char flag[4];
            strcpy(flag, "YES");
            num0 = sqrt(num);
            for (long long i = 2; i <= num0; i++){
                if ((num % i) == 0){
                    strcpy(flag, "NO");
                    break;
                }
            }
            printf("%s\n", flag);
            break;
        case 3:
            long long check = 0;
            for(long long i = 0; i < strlen(argv[1]); i++){
                if (argv[1][i] == '0' && check == 0) continue;
                else{
                    check = 1;
                    printf("%c ", argv[1][i]);
                }
            }
            printf("\n");
            break;
        case 4:
            if (num > 10 || num < 1){
                printf("Введённое число дожно быть от 1 до 10\n");
                break;
            }
            long long ans;
            printf("_________________________");
            for (long long i = 0; i < num-1; i++){
                printf("_____________");
            }
            printf("\n");
            printf("|Степень/основание\t|");
            for (long long i = 1; i < num; i++){
                printf("%12d|", i);
            }
            printf("\n");
            printf("_________________________");
            for (long long i = 0; i < num-1; i++){
                printf("_____________");
            }
            printf("\n");
            for (long long i = 1; i < 10; i++){
                printf("|%23d|", i);
                for (long long j = 1; j < num; j++){
                    ans = pow(i, j);
                    printf("%12d|", ans);
                }
                printf("\n");
                printf("_________________________");
                for (long long i = 0; i < num-1; i++){
                    printf("_____________");
                }
                printf("\n");
            }
            break;
        case 5:
            if (num < 1){
                printf("Введённое число должно быть больше 0\n");
                break;
            }
            long long answ = 0;
            answ = (num + 1) * num / 2; 
            if (ans < num){
                printf("Переполнение\n");
                break;
            }
            printf("%lld\n", answ);
            break;
        case 6:
            if (num < 1){
                    printf("Введённое число должно быть положительным\n");
                    break;
                }
            if (num > 20){
                printf("Переполнение\n");
                break;
            }
            printf("%lld\n", fact(num));
            break;
        case 0:
            printf("Данный флаг не найден\n");
            break;
    }
}
    