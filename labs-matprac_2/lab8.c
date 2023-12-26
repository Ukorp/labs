#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

char array[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef enum errors{
    ok, fail, memory_allocation_problem,
    overflow, bad_system, bad_number
}errors;

errors define_overflow_longlong(long long a){

    if ((a >= LLONG_MAX) || (a <= LLONG_MIN)) {
        return overflow;
    }
    else return ok;
}

double find_eps(){
    double x = 1;
    int i = 0;
    while (x + 1.0 > 1.0){
        x /= 2;
        i++;
    }
    return x;
}

void reverse(char ** str){
    char tmp;
    int size = strlen(*str);
    for (int i = 0; i < (size / 2); i++){
        tmp = (*str)[i];
        (*str)[i] = (*str)[size - 1 - i];
        (*str)[size - 1 - i] = tmp;
    }
}

int find_index (char k){
    if (((k < '0') || (k > '9')) && ((k < 'A') || (k > 'Z'))){
        return -1;
    }
    int l = 0;
    int r = 36;
    int mid;
    while (r - l != 0){
        mid = (r + l) / 2;
        if (array[mid] == k) return mid;
        else if (array[mid] > k)
            r = mid;
        else l = mid;
        
    }
    return r;
}

errors sum_2(char ** str1, char * str2, int number_system){
    char * tmp1, * tmp2;
    if (*str1 == NULL) return fail; 
    tmp1 = strdup(*str1);
    if (tmp1 == NULL){
        free(tmp1);
        return memory_allocation_problem;
    }
    tmp2 = strdup(str2);
    if (tmp1 == NULL){
        free(tmp1);
        free(tmp2);
        return memory_allocation_problem;
    }
    reverse(&tmp1);
    reverse(&tmp2);
    int sum = 0;
    int tmp;
    int append = 0;
    while (strlen(tmp1) < strlen(tmp2)){
        strcat(tmp1, "0");
    }
    while (strlen(tmp2) < strlen(tmp1)){
        strcat(tmp2, "0");
    }
    char * answer = (char *)malloc(strlen(tmp1) * 2 * sizeof(int));
    if (answer == NULL){
        free(tmp1);
        free(tmp2);
        return memory_allocation_problem;
    }
    char cat[2];
    answer[0] = '\0';
    for (int i = 0; i < strlen(tmp1); ++i){
        if ((find_index(toupper(tmp1[i])) == -1) || (find_index(toupper(tmp2[i])) == -1)){
            free(tmp1);
            free(tmp2);
            return bad_number;
        }
        tmp = find_index(toupper(tmp1[i])) + find_index(toupper(tmp2[i])) + append;
        append = tmp / number_system;
        
        cat[0] = array[tmp % number_system];
        cat[1] = '\0';
        // printf("%s %s %s\n", answer, tmp1, tmp2);
        strcat(answer, cat);
    }
    cat[0] = array[append];
    strcat(answer, cat);
    int count = strlen(answer) - 1;
    while (answer[count] == '0'){
        answer[count--] = '\0';
    }
    free(tmp1);
    free(tmp2);
    reverse(&answer);
    *str1 = strdup(answer);
    if (*str1 == NULL){
        free(answer);
        return memory_allocation_problem;
    }
    free(answer);
    return ok;
}

int check_minimal_base(char * str){
    int max = 0;
    while (*str){
        int base = find_index(*str);
        if (base == -1) return 0;
        if (base > max) max = base;
        str++;
    }
    return max;
}

char * sum_numbers(errors * status_code, int number_system, int quantity, ...){
    va_list number;
    int size = 128;
    if (number_system < 2 || number_system > 36) {
        *status_code = bad_system;
        return NULL;
    }
    char * answer = malloc(sizeof(char));
    if (answer == NULL){ 
        *status_code = memory_allocation_problem;
        return NULL;
    }
    char * tmp;
    va_start(number, quantity);
    if (quantity == 1){
        *status_code = memory_allocation_problem;
        free(answer);
        return va_arg(number, char *);
    }
    for (int i = 0; i < quantity; i++){
        if (i == 0){
            free(answer);
            answer = strdup(va_arg(number, char *));
            
            if (answer == NULL){
                *status_code = memory_allocation_problem;
                return NULL;
            }
            if (check_minimal_base(answer) + 1 > number_system){
                free(answer);
                *status_code = bad_number;
                return NULL;
            }
            continue;
        }
        tmp = va_arg(number, char *);
        switch(sum_2(&answer, tmp, number_system)){
            case memory_allocation_problem:
                *status_code = memory_allocation_problem;
                free(answer);
                return NULL;
            case bad_number:
                *status_code = bad_number;
                free(answer);
                return NULL;   
            case ok:
                break;
        }
        if (answer == NULL) return NULL;
    }
    va_end(number);

    if (answer[0] == '\0'){
        answer[0] = '0';
        answer[1] = 0;
    }
    return answer;
}



int main()
{   
    errors status_code = ok;
    char * b = sum_numbers(&status_code, 36, 3, "-1", "0", "1");
    switch (status_code)
    {
    case bad_number: 
        puts("Заданное в аргументе число невозможно представить в заданной системе счисления");
        free(b);
        return bad_number;
    case memory_allocation_problem: 
        puts("Заданное в аргументе число невозможно представить в заданной системе счисления");
        free(b);
        return memory_allocation_problem;
    case bad_system: 
        puts("Заданная система счисления должна быть между 2 и 36");
        free(b);
        return memory_allocation_problem;
    case ok:
        printf("%s\n", b);
        free(b);
        return ok; 
    default:
        puts("Неизвестная ошибка");
        free(b);
        return fail;
    }
    return 0;
}
