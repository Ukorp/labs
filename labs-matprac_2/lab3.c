#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum errors{
    ok, not_ok, no_flag, argument_error,
    unknown_error, wrong_int, overflow,
    memory_allocation_problem, fail
}errors;

typedef struct pair{
    int index;
    int substr;
}pair;

errors sdvig(char * str, char k){
    if (k == EOF) return fail;
    for (int i = 1; i < strlen(str); ++i){
        str[i - 1] = str[i];
    }
    str[strlen(str) - 1] = k;
    return ok;
}

errors equal_str(char * str1, char * str2){
    if (strlen(str1) != strlen(str2)) return fail;
    int size = strlen(str1);
    for (int i = 0; i < size / 2; ++i){
        if ((str1[i] != str2[i]) || (str1[size - i] != str2[size - i])) return fail;
    }
    if (str1[size / 2] != str2[size / 2]) return fail;
    return ok;
}

pair * find_substr(FILE * file, char * substr, errors * status_code){
    int string = 1;
    char finding[strlen(substr) + 1];
    int counting_previous = 1;
    int counting = 0;
    char k = 1;
    int i = 0;
    pair * result = malloc(sizeof(pair));
    for (int j = 0; j < strlen(substr); ++j){
        finding[j] = fgetc(file);
    }
    do {
        if (equal_str(finding, substr) == ok){
            *status_code = ok;
            if ((counting - (int)strlen(substr) + 1) > 0){
                pair answer = {counting - strlen(substr) + 1, string};
                result[i++] = answer;
                pair * ptr = realloc(result, sizeof(pair) * (i + 1));
                if (ptr == NULL){
                    free(result);
                    return NULL;
                }
                result = ptr;
            }
            else{
                string--;
                pair answer = {counting_previous + counting - strlen(substr) + 1, string};
                result[i++] = answer;
                pair * ptr = realloc(result, sizeof(pair) * (i + 1));
                if (ptr == NULL){
                    free(result);
                    return NULL;
                }
                result = ptr;
            }
        }
        k = fgetc(file);
        counting++;
        if (k == '\n'){
            string++;
            counting_previous = counting;
            counting = 0;
        }
    }while (sdvig(finding, k) != fail);
    *status_code = fail;
    result[i].index = -1;
    if (i == 0) return NULL;
    return result;
    
}

errors file_substring(pair *** array, char * str, int argc, ...){
    *array = malloc(sizeof(pair *) + 1);
    va_list list;
    pair * answer;
    FILE * file;
    errors status_code = ok;
    va_start(list, argc);
    for(int i = 0; i < argc; ++i){
        file = fopen(va_arg(list, char *), "r+");
        answer = find_substr(file, str, &status_code);
        (*array)[i] = answer;
        pair ** ptr = realloc(*array, sizeof(pair) * (i + 1) + 1);
        if (ptr == NULL){ 
            return memory_allocation_problem;
        }
        *array = ptr;
        fclose(file);
    }
    (*array)[argc] == NULL;
    return ok;
}

void free_pair(int argc, pair ** ans){
    for (int i = 0; i < argc; ++i){
        free(ans[i]);
    }
    free(ans);
}

void print_result(int argc, char * argv[], pair ** ans){
    for (int i = 0; i < argc; ++i){
        int j = 0;
        printf("Файл %s: \n\n", argv[i + 1]);
        if (ans[i] == NULL){
            puts("Вхождения не найдены");
            continue;
        }
        while (ans[i][j].index != -1){
            printf("Позиция первого символа в строке: %d\n", ans[i][j].index);
            printf("Номер строки: %d\n", ans[i][j++].substr);
            puts("");
        }
    }
}



int main(int argc, char * argv[]){
    if (argc != 3) {
        puts("Неверное число аргументов\n");
        return argument_error;
    }
    char buff[1024];
    int i = 1;
    pair ** ans;
    errors status_code = ok;
    char substr[150] = "sda";
    switch (file_substring(&ans, substr, argc-1, argv[1], argv[2])){
        case memory_allocation_problem:
            free_pair(argc-1, ans);
            printf("Ошибка выделения памяти\n");
            return memory_allocation_problem;
        case ok:
            print_result(argc-1, argv, ans);
            free_pair(argc-1, ans);
            return 0;
    }
    return ok;
}