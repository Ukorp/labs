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

enum flags{
    r, a, no_flag
};

enum flags find_flag(char * str){
    if (strlen(str) < 2) return no_flag;
    if (str[0] != '-' && str[0] != '/') return no_flag;
    if (str[1] == 'a') return a;
    if (str[1] == 'r') return r;
    return no_flag;
}

void flag_r(FILE * file1, FILE * file2, FILE * file3){
    char ptr1, ptr2;

    while (((ptr1 = getc(file1)) != EOF) && ((ptr2 = getc(file2)) != EOF)){
        while(ptr1 < '!' && ptr1 != EOF)
            ptr1 = getc(file1);
        if (ptr1 == EOF) break;
        fprintf(file3,"%c ", ptr1);

        while(ptr2 < '!' && ptr2 != EOF)
            ptr2 = getc(file2);
        if (ptr2 == EOF) break;
        fprintf(file3, "%c ", ptr2);

    }
    while ((ptr1 = getc(file1)) != EOF){
        while(ptr1 >= 0 && ptr1 < '!' && ptr1 != EOF)
            ptr1 = getc(file1);
        if (ptr1 == EOF) break;
        fprintf(file3,"%c ", ptr1);
    }
    while ((ptr2 = getc(file2)) != EOF){
        while(ptr2 >= 0 && ptr2 < '!' && ptr2 != EOF)
            ptr2 = getc(file2);
        if (ptr2 == EOF) break;
        fprintf(file3, "%c ", ptr2);
    }
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

void itoa4(int num, char * str){
    int i = 0;
    char num_char;
    while (num != 0){

        num_char = (num % 4) + '0';
        str[i] = num_char;
        num /= 4;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

void itoa8(int num, char * str){
    int i = 0;
    char num_char;
    while (num != 0){

        num_char = (num % 8) + '0';
        str[i] = num_char;
        num /= 8;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

void flag_a(FILE * in, FILE * out){
    char ptr;
    int n = 0;
    char * str = (char *)malloc(128);
    while ((ptr = getc(in)) != EOF){
        while(ptr < '!' && ptr != EOF){
            ptr = getc(in);
        }
        if (ptr == EOF) return;
        n++;
        if (n % 10 == 0){
            if (ptr >= 'A' && ptr <= 'Z'){
                ptr += ('a' - 'A');
            }
            if (ptr >= 'a' && ptr <= 'z'){
                itoa4((int)ptr, str);
                fprintf(out, "%s ", str);
            }
        }
        else if (n % 2 == 0){
            if (ptr >= 'A' && ptr <= 'Z'){
                ptr += ('a' - 'A');
            }
            fprintf(out, "%c ", ptr);
        }
        else if (n % 5 == 0){
            itoa8((int)ptr, str);
            fprintf(out, "%s ", str);
        }
        else fprintf(out, "%c ", ptr);
        
    }
    free(str);

}

int main(int argc, char * argv[]){
    setlocale(LC_ALL, "ru");
    if (argc < 2) {
        printf("Ошибка\n");
        return wrong_argument_quantity;
    }
    FILE * file1;
    FILE * file2;
    FILE * file3;

    switch(find_flag(argv[1])){
        case r:
            if (argc != 5){
                printf("Неправильно поданы аргументы\n");
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(argv[3], "r")) || !(file3 = fopen(argv[4], "w+"))){
                fcloseall();
                printf("Ошибка открытия файла\n");
                return file_error;
            }
            flag_r(file1, file2, file3);
            printf("Выполнено\n");
            break;
        case a:
            if (argc != 4){
                printf("Неправильно поданы аргументы\n");
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(argv[3], "w+"))){
                fcloseall();
                printf("Ошибка открытия файла\n");
                return file_error;
            }
            flag_a(file1, file2);
            printf("Выполнено\n");
            break;
        case no_flag:
            printf("Флаг не найден\n");
            return no_flag;
        default:
            printf("Флаг не найден\n");
            return no_flag;
    }
    fcloseall();
    return ok;
}