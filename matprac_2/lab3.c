#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef enum errors{
    ok, not_ok, no_flag, argument_error,
    unknown_error, wrong_int, overflow,
    memory_allocation_problem
}errors;

int * Zfunc(char * str, errors * status_code){
    int size = strlen(str);
    int * a = (int *)malloc(sizeof(int) * size);
    if (a == NULL){
        *status_code = memory_allocation_problem;
        return a;
    }
    a[0] = -1;
    for (int i = 1; i < size; ++i){
        int k = 0;
        for (int j = 0; j < i; ++j){
            if (str[j] != str[i + j]){
                a[i] = k;
                break;
            }
            k++;
        }
        a[i] = k;
    }
    return a;
}

int find_substring(char * str, char * substr, errors * status_code, int * string){
    char unknown_char[2];
    unknown_char[0] = 255;
    unknown_char[1] = '\0';
    *string = 1;
    int size0 = strlen(str) + strlen(substr) + 1;
    char * tmp = (char *)malloc(sizeof(char) * size0);
    if (tmp == NULL){
        *status_code = memory_allocation_problem;
        return memory_allocation_problem;
    }
    strcpy(tmp, substr);
    strcat(tmp, unknown_char);
    strcat(tmp, str);
    int * a = Zfunc(tmp, status_code);
    int size = strlen(substr);
    int count = 0;
    for (int i = size; i < strlen(tmp); ++i){
        count++;
        
        if (a[i] >= size) {
            free(a);
            free(tmp);
            return count;
        }
        if (tmp[i] == '\n') {
            (*string)++;
            count = 0;
        }
    }
    free(a);
    free(tmp);
    return -1;
    
}

int file_substring(char * str, ...){
    
}

errors file_to_string(char ** str, FILE * file){
    char * tmp;
    char k;
    int i = 1;
    while ((k = fgetc(file)) != EOF){
        i++;
        tmp = (char *)realloc(*str, sizeof(char) * i);
        if (tmp == NULL) return memory_allocation_problem;
        *str = tmp;
        (*str)[i-2] = k;
    }
    *str[i-1] = 0;
    return ok;
}

int main(int argc, char * argv[]){
    char buff[1024];
    int i = 1;
    char * str = (char *)malloc(sizeof(char) * 1);
    FILE * file;
    file = fopen("hz.txt", "r");
    if (file_to_string(&str, file) == memory_allocation_problem) {
        puts("Memory allocation problem\n");
        free(str);
        return memory_allocation_problem;
    };
    errors status_code = ok;
    char substr[150] = "exit";
    int answer = find_substring(str, substr, &status_code, &i);
    if (status_code == memory_allocation_problem){
        free(str);
        printf("Ошибка выделения памяти\n");
        return memory_allocation_problem;
    }
    printf("Позиция первого символа подстроки: %d\n", answer);
    printf("Номер строки: %d\n", i);
    return ok;
}