#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char array[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef enum errors{
    ok, fail, memory_allocation_problem
}errors;

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
    tmp1 = strdup(*str1);
    tmp2 = strdup(str2);
    
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
    if (answer == NULL) return fail;
    char cat[2];
    answer[0] = '\0';
    for (int i = 0; i < strlen(tmp1); ++i){
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
    free(*str1);
    *str1 = strdup(answer);
    free(answer);
    return ok;
}

char * sum_numbers(int number_system, int quantity, ...){
    va_list number;
    int size = 128;
    
    char * answer;
    char * tmp;
    va_start(number, quantity);
    if (quantity == 1) return va_arg(number, char *);
    for (int i = 0; i < quantity; i++){
        if (i == 0){
            answer = strdup(va_arg(number, char *));
            continue;
        }
        tmp = va_arg(number, char *);
        sum_2(&answer, tmp, number_system);
        if (answer == NULL) return NULL;
    }
    va_end(number);
    return answer;
    
}

int main()
{   
    char * b = sum_numbers(16, 3, "A", "A", "A");
    if (b == NULL){
        puts("Memory allocation problem\n");
        return memory_allocation_problem;
    }
    printf("%s\n", b);
    free(b);
    return 0;
}
