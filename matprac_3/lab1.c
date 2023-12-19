#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    ok, memory_allocation_problem, overflow
}errors;

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
    if (number > INT_MAX) return overflow;
    int i = 0;
    char * ptr;
    int tmp = 1;
    char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < r-1; i++){
        tmp <<= 1;
        tmp |= 1;
    }
    while (number > 0){
        ptr = (char *)realloc(str, sizeof(char) * (i + 1) );
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
    switch(ten_to_r(147, 5, &str)){
        case overflow:
            printf("Переполнение\n");
            free(str);
            return overflow;
        case memory_allocation_problem:
            printf("Ошибка выделения памяти\n");
            free(str);
            return overflow;
        case ok:
            printf("%s\n", str);
            free(str);
            return ok;
        default:
            
    };

    free(str);
    return 0;
}
