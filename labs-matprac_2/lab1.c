#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef enum errors{
    ok, no_ok, no_flag, argument_error,
    unknown_error, wrong_int, overflow,
    memory_allocation_problem
}errors;

typedef enum flags{
    l = no_flag + 1, r, u, n, c
}flags;



flags define_flag(char * str){
    if ((str[0] == '-') && (str[2] == 0)){
        if (str[1] == 'l') return l;
        if (str[1] == 'r') return r;
        if (str[1] == 'u') return u;
        if (str[1] == 'n') return n;
        if (str[1] == 'c') return c;
        return no_flag;
    }
    return no_flag;
}

int len(char * str){
    int i = 0;
    while (str[i] != 0) i++;
    return i;
}

void reverse(char * str){
    int size = len(str);
    char tmp;
    for (int i = 0; i < (size / 2); ++i){
        tmp = str[i];
        str[i] = str[size - i - 1];
        str[size - i - 1] = tmp;
    }
    return;
}

errors stradd(char * str1, char * str2){
    int size1 = len(str1);
    int size2 = len(str2);
    char * ptr = (char *)realloc(str1, sizeof(char) * (size1 + size2));
    if (ptr == NULL) return memory_allocation_problem;
    str1 = ptr;
    for (int i = size1; i < (size1 + size2); i++){
        str1[i] = str2[i-size1];
    }
    return ok;
}



void upper(char * str){
    int i = 0;
    while (str[i] != 0){
        if (str[i] >= 'a' && str[i] <= 'z' && i % 2 == 0){
            str[i] = str[i] - ('a' - 'A');
        }
        i++;
    }
} 



char * swap(char * str, char * ans){
    int k = 0;
    int size = len(str);
    for (int i = 0; i < size; i++){
        if (str[i] <= '9' && str[i] >= '0'){
            ans[k] = str[i];
            k++;
        }
    }
    for (int i = 0; i < size; i++){
        if ((str[i] <= 'z' && str[i] >= 'a') || (str[i] <= 'Z' && str[i] >= 'A')){
            ans[k] = str[i];
            k++;
        }
    }
    for (int i = 0; i < size; i++){
        if ((str[i] > 'z' || str[i] < 'a') && (str[i] > 'Z' || str[i] < 'A') && (str[i] < '0' || str[i] > '9')){
            ans[k] = str[i];
            k++;
        }
    }
    ans[size] = 0;
    if (k == size) return ans;
    else return NULL;
}

int is_int(char * str){
    for (int i = 0; i < len(str); i++){
        if ((str[i] == '-') && (i == 0)) return wrong_int;
        if (str[i] < '0' || str[i] > '9') 
            return wrong_int;
    }
    return ok;
}

int * create_array(int argc){
    int * arr = (int *)malloc(sizeof(int) * (argc - 3));
    if (arr == NULL) return NULL;
    // int arr[argc - 3];
    for (int i = 0; i < argc - 3; i++){
        arr[i] = (i + 3);
    }
    return arr;
}

int shuffle(int * arr, int argc, unsigned long long un){
    srand(un);
    int tmp, random;
    for (int i = 0; i < (argc - 3); i++){
        random = rand() % (argc - 3);
        tmp = arr[i];
        arr[i] = arr[random];
        arr[random] = tmp;
    }
}

int main(int argc, char * argv[])
{
    if (argc < 3) {
        printf("Флаг отсутствует\n");
        return argument_error;
    }
    unsigned long long int un;
    char * end;
    char * ans = malloc(sizeof(char) * len(argv[2]));
    switch(define_flag(argv[1])){
        case l:
            if (argc < 3) {
                printf("Строка отсутствует\n");
                free(ans);
                return argument_error;
            }
            printf("Размер данной строки: %d символов\n", len(argv[2]));
            free(ans);
            return ok;
        case r:
            if (argc < 3) {
                free(ans);
                printf("Строка отсутствует\n");
                return argument_error;
            }
            reverse(argv[2]);
            printf("Перевёрнутая строка: %s\n", argv[2]);
            free(ans);
            return ok;
        case u:
            if (argc < 3) {
                printf("Строка отсутствует\n");
                free(ans);
                return argument_error;
            }
            upper(argv[2]);
            printf("Полученная строка: %s\n", argv[2]);
            free(ans);
            return ok;
        case n:
            
            if (argc < 3) {
                printf("Строка отсутствует\n");
                free(ans);
                return argument_error;
            }
            if (swap(argv[2], ans) == NULL){
                printf("Неизвестная ошибка\n");
                free(ans);
                return unknown_error;
            };
            printf("Полученная строка: %s\n", ans);
            free(ans);
            return ok;
        case c:
            if (argc < 3) {
                printf("Строка отсутствует\n");
                free(ans);
                return argument_error;
            }
            if (is_int(argv[2]) != ok){
                printf("Ошибка аргументов!!!\n");
                free(ans);
                return wrong_int;
            }
            un = strtol(argv[2], &end, 10);
            if (un > ULONG_MAX){
                printf("Переполнение\n");
                free(ans);
                return overflow; 
            }
            int * count = create_array(argc);
            char * result = (char *)malloc(sizeof(char) * 25);
            if (result == NULL || count == NULL){
                free(result);
                free(count);
                free(ans);
                puts("Memory allocation problem");
                return memory_allocation_problem;
            }
            shuffle(count, argc, un);
            for (int i = 0; i < (argc - 3); i++){
                if (stradd(result, argv[count[i]]) == memory_allocation_problem){
                    free(count);
                    free(result);
                    free(ans);
                    return memory_allocation_problem;
                }
            }
            printf("Полученная строка: %s\n", result);
            free(count);
            free(result);
            free(ans);
            return ok;
        case no_flag:
            printf("Данного флага не существует\n");
            free(ans);
            return no_flag;

    }

    return ok;
}
