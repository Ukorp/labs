#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    ok, memory_allocation_problem, overflow,
    unknown_error, numeral_system_error
}errors;

errors define_overflow_int(int a){

    if ((a >= INT_MAX) || (a <= INT_MIN)) {
        return overflow;
    }
    else return ok;
}

void reverse(char * str){
    int size = strlen(str);
    char tmp;
    for (int i = 0; i < (size >> 1); ++i) {
        tmp = str[i];
        str[i] = str[size - 1 - i];
        str[size - 1 - i] = tmp;
    }
}

errors ten_to_r(int number, int r, char ** str){
    if (number < 0) return overflow;
    if ((r < 1) || (r > 5)) return numeral_system_error;
    int i = 0;
    char * ptr;
    int tmp = 1;
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < r-1; i++){
        tmp <<= 1;
        tmp |= 1;
    }
    while (number > 0){
        ptr = (char *)realloc(*str, sizeof(char) * (i + 1) );
        if (ptr == NULL) return memory_allocation_problem;
        *str = ptr;
        (*str)[i++] = alphabet[(number & tmp)];
        number >>= r;
    }
    (*str)[i] = '\0';
    reverse(*str);
    return ok;
    
}

int main()
{
    char * str = (char *)malloc(sizeof(char));
    if (str == NULL){
        puts("Ошибка выделения памяти\n");
        return memory_allocation_problem;
    }
    switch(ten_to_r(12321313213213123213213213132133, 1, &str)){
        case overflow:
            printf("Переполнение\n");
            free(str);
            return overflow;
        case memory_allocation_problem:
            printf("Ошибка выделения памяти\n");
            free(str);
            return overflow;
        case ok:
            printf("ok %s\n", str);
            free(str);
            return ok;
        case numeral_system_error:
            printf("Неверная система счисления\n");
            free(str);
            return ok;
        default:
            printf("Неизвестная ошибка\n");
            free(str);
            return unknown_error;
    }
    free(str);
    return unknown_error;
}
