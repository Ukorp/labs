#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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

long long is_int(char * str){
    
    for (long long i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return 0;
    }
    return 1;
}

void reverse(char * str){
    if (str == NULL) return;
    char * end = str + (strlen(str) - 1);
    while (str <= end){
        char c = *str;
        *str = *end;
        *end = c;
        str++;
        end--;
    }
}

long long Nto10(char * in, long long base, int * overflow_detection){
    long long ans, pw;
    pw = 1;
    long long tmp;
    ans = 0;
    long long size = strlen(in);
    reverse(in);
    for (long long i = 0; i < strlen(in); ++i){
        if (in[i] >= 'A' && in[i] <= 'Z'){
            tmp = in[i] - 'A' + 10;
        }
        else if (in[i] >= 'a' && in[i] <= 'z'){
            tmp = in[i] - 'a' + 10;
        }
        else
            tmp = in[i] - '0';
        ans += tmp * pow(base, i);
    }
    reverse(in);
    if (define_overflow_longlong(ans) == overflow){
        *overflow_detection = overflow;
        return overflow;
    }
    return ans;
}

errors itobase(long long num, long long base, char * str, int * size){
    long long i = 0;
    char num_char;
    if (num == 0) {
        strcpy(str, "0");
        return ok;
    }
    while (num != 0){

        num_char = (num % base <= 9) ? (num % base) + '0' : (num % base) - 10 + 'a';
        if ((i + 1) > *size){
            *size++;
            str = (char *)realloc(str, sizeof(char) * *size);
        }
        if (str == NULL){
            return memory_allocation_problem;
        }
        str[i] = num_char;
        num /= base;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

long long this_base(char * str, long long base){
    char k;
    
    for (long long i = 0; i < strlen(str); i++){
        k = str[i];
        // printf("%c == %d\n", k, base - 1);
        if (k >= '0' && k <= '9'){
            if ((k - '0') > (base - 1)) return 0;
        }
        else if (k >= 'a' && k <= 'z'){
            if (((k - 'a') + 10) > (base - 1)) return 0;
        }
        else if (k >= 'A' && k <= 'Z'){
            if (((k - 'A') + 10) > (base - 1)) return 0;
        }
        else return 0;
    }
    return 1;
}

void delete_0(char * str){
    long long i, j;
    i = 0;
    j = 0;
    while (str[j] == '0') j++;
    if (str[j] == 0){
        str[0] = '0';
        str[1] = 0;
        return;
    }
    while (str[j] != 0){
        str[i] = str[j];
        i++;
        j++;
    }
    str[i] = 0;

}

int main(){
    long long in;
    int flow = 0;
    int size = 256;
    char * str = (char *)malloc(sizeof(char) * size);
    char * ans = (char *)malloc(sizeof(char) * size);
    char * out = (char *)malloc(sizeof(char) * size);
    long long max = -1;
    scanf("%d", &in);
    if ((in > 36) || (in < 2)){
        printf("Введена неверная система счисления\n");
        free(str);
        free(ans);
        free(out);
        return wrong_arguments;
    }
    if (str == NULL || ans == NULL || out == NULL){
        free(str);
        free(ans);
        free(out);
        printf("Ошибка выделения памяти\n");
        return memory_allocation_problem;
    }
    long long count = 0;
    scanf("%s", str);
    if (strcmp(str, "Stop") == 0){
        free(str);
        free(ans);
        free(out);
        printf("Числа не были введены\n");
        return wrong_arguments;
    }
    while (strcmp(str, "Stop") != 0){
        
        if (this_base(str, in) == 0){
            printf("Число %s не существует в %d системе счисления\n", str, in);
        }
        else{
            delete_0(str);
            if (max < (count = Nto10(str, in, &flow))){
                max = count;
                strcpy(ans, str);
            }
        }
        scanf("%s", str);
    }
    strcpy(str, ans);
    if (str[0] == 0){
        free(str);
        free(ans);
        free(out);
        printf("Подходящие числа не были введены\n");
        return wrong_arguments;
    }
    if (itobase(Nto10(str, in, &flow), 9, out, &size) == memory_allocation_problem){
        printf("Ошибка выделения памяти\n");
        free(str);
        free(ans);
        free(out);
        return memory_allocation_problem;
    };
    if (flow == overflow){
        printf("Переполнение\n");
        free(str);
        free(ans);
        free(out);
        return overflow;
    }
    printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, out, 9);
    if (itobase(Nto10(str, in, &flow), 18, out, &size) == memory_allocation_problem){
        printf("Ошибка выделения памяти\n");
        free(str);
        free(ans);
        free(out);
        return memory_allocation_problem;
    };
    printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, out, 18);
    if (itobase(Nto10(str, in, &flow), 27, out, &size) == memory_allocation_problem){
        printf("Ошибка выделения памяти\n");
        free(str);
        free(ans);
        free(out);
        return memory_allocation_problem;
    };
    printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, out, 27);
    if (itobase(Nto10(str, in, &flow), 36, out, &size) == memory_allocation_problem){
        printf("Ошибка выделения памяти\n");
        free(str);
        free(ans);
        free(out);
        return memory_allocation_problem;
    };
    printf("%s в %d системе счисления = %s в %d системе счисления\n\n",str, in, out, 36);
    count++;
    free(str);
    free(ans);
    free(out);
    return ok;
    
}