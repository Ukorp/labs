#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

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

void itoa(long long num, char * str, long long osnovanie){
    long long i = 0;
    char num_char;
    if (num == 0){
        str[0] = '0';
        str[1] = 0;
        return;
    }
    while (num != 0){

        num_char = (num % osnovanie <= 9) ? (num % osnovanie) + '0' : (num % osnovanie) - 10 + 'A';
        str[i] = num_char;
        num /= osnovanie;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

long long max_to_int(char ptr){
    char k;
    if (ptr >= 'A' && ptr <= 'Z'){
        k = ptr - 'A' + 10;
    }
    if (ptr >= 'a' && ptr <= 'z'){
        k = ptr - 'a' + 10;
    }
    if (ptr >= '0' && ptr <= '9'){
        k = ptr - '0';
    }
    return (long long)k;
}

long long do_work(FILE * in, FILE * out, int  * overflow_detection){
    int size = 256;
    char ptr;
    char max = '!';
    char min = 'z' + 'Z';
    char * str = (char *)malloc(sizeof(char) * size);
    if (str == NULL){
        printf("Память не выделена\n");
        return memory_allocation_problem;
    }
    long long i = 0;
    ptr = '!';
    char * tmp;
    while (ptr  != EOF){
        if (ptr < '!' && ptr != EOF){
            while(ptr < '!' && ptr != EOF){
                ptr = getc(in);
            }
        }
        else ptr = getc(in);
        if ((ptr < '0' || ptr > '9') && (ptr < 'A' || ptr > 'Z') && (ptr < 'a' || ptr > 'z') && (ptr >= '!')){
            free(str);
            return wrong_argument_quantity;
        }
        if ((ptr >= '0' && ptr <= '9') || (ptr >= 'A' && ptr <= 'Z') || (ptr >= 'a' && ptr <= 'z')){
            if (ptr >= 'a' && ptr <= 'z') ptr = ptr - 'a' + 'A'; 
            if ((i + 1) > size ){
                size++;
                tmp = (char *)realloc(str, sizeof(char) * size);
                if (tmp == NULL){
                free(str);
                return memory_allocation_problem;
            }
            str = tmp;
            }

            str[i] = ptr;
            if (ptr > max) max = ptr;
            i++;
        }
        else{
            str[i] = 0;
            char * temp = str;
            while (*temp == '0') temp++;
            if (*temp == '\0') temp--;
            fprintf(out, "%15s%15lld%15lld\n", temp, (max_to_int(max) != 0) ? (max_to_int(max) + 1) : 2, Nto10(str, (max_to_int(max) + 1), overflow_detection));
            if (*overflow_detection == overflow){
                free(str);
                return overflow;
            }
            i = 0;
            max = '!';
        }

        if (ptr == EOF) {
            // free(str);
            return ok;
        }
        

    }
    free(str);
    return ok;
    
}

long main(long long argc, char * argv[]){
    FILE * file1, * file2;
    int a = 0;
    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "w+");
    if (file1 && file2){
        if (do_work(file1, file2, &a) != ok){
            printf("Неправильно поданы данные\n");
            fclose(file1);
            fclose(file2);
            return wrong_arguments;
        }
        else printf("Выполнено\n");
    }
    else{
        if (file1 != NULL) fclose(file1);
        if (file2 != NULL) fclose(file2);
        printf("Входной файл не найден\n");
        return file_error;
    }
}