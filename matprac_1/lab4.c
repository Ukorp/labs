#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>


typedef enum errors{
    ok, memory_allocation_problem, wrong_int,
    wrong_double, wrong_argument_quantity,
    wrong_arguments, overflow, no_x,
    file_error

}errors;

enum flags{
    d, i, s, a, no_flag,
    nd, ni, ns, na
};

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

void itoa16(int num, char * str){
    int i = 0;
    char num_char;
    
    while (num != 0){

        num_char = (num % 16 <= 9) ? (num % 16) + '0' : (num % 16) - 10 + 'A';
        str[i] = num_char;
        num /= 16;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

void find_d(FILE * in, FILE * out){
    char  k;
    while ((k = getc(in)) != EOF){
        if (k < '0' || k > '9') putc(k, out);
    }
}

void find_s(FILE * in, FILE * out){
    char  k;
    int count = 0;
    while ((k = getc(in)) != EOF){
        if ((k < '0' || k > '9') && 
        ((k < 'a' || k > 'z') && (k < 'A' || k > 'Z')) &&
        (k != ' ') && (k != '\n'))
            count++;
    }
    fprintf(out, "%d", count);
}

void find_a(FILE * in, FILE * out){
    char  k;
    char * str = (char *)malloc(128);
    while ((k = getc(in)) != EOF){
        if (k >= '0' && k <= '9') putc(k, out);
        else{
            itoa16((int)k, str);
            fprintf(out, "%s",str);
        }
    }
}

void find_i(FILE * in, FILE * out){
    char k;
    int count = 0;
    while ((k = getc(in)) != EOF){
        if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')) 
            count++;
    }
    
    fprintf(out, "%d", count);
}

enum flags define_flag(char * fl){
    if (strlen(fl) < 2) return no_flag;
    if (fl[0] != '-' && fl[0] != '/') return no_flag;
    if (strlen(fl) == 3){
        if (fl[1] == 'n'){
            if (fl[2] == 'd') return nd;
            if (fl[2] == 'i') return ni;
            if (fl[2] == 's') return ns;
            if (fl[2] == 'a') return na;
        }
    }
    else if (strlen(fl) == 2) {
        if (fl[1] == 'd') return d;
        if (fl[1] == 'i') return i;
        if (fl[1] == 's') return s;
        if (fl[1] == 'a') return a;
    }
    return no_flag;
    
}

int main(int argc, char * argv[]){
    if (argc < 2) {
        printf("Ошибка\n");
        return wrong_argument_quantity;
    }
    FILE * file1;
    FILE * file2;
    char * out = (char *)malloc(sizeof(char) * 128);
    if (out == NULL){
        printf("Ошибка выделения памяти\n");
        free(out);
        return memory_allocation_problem;
    }
    strcpy(out, "out_");
    switch (define_flag(argv[1])){
        case d:
            if (argc != 3){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            strcat(out, argv[2]);
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_d(file1, file2);
            printf("Выполнено\n");
            break;
        case i:
            if (argc != 3){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            strcat(out, argv[2]);
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_i(file1, file2);
            printf("Выполнено\n");
            break;
        case s:
            if (argc != 3){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            strcat(out, argv[2]);
            if ((file1 = fopen(argv[2], "r")) == 0 || (file2 = fopen(out, "w+")) == 0) {
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_s(file1, file2);
            printf("Выполнено\n");
            break;
        case a:
            if (argc != 3){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            strcat(out, argv[2]);
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_a(file1, file2);
            printf("Выполнено\n");
            break;
        case nd:
            if (argc != 4){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_d(file1, file2);
            printf("Выполнено\n");
            break;
        case ni:
            if (argc != 4){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_i(file1, file2);
            printf("Выполнено\n");
            break;
        case ns:
            if (argc != 4){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_s(file1, file2);
            printf("Выполнено\n");
            break;
        case na:
            if (argc != 4){
                printf("Неверное количество аргументов\n");
                free(out);
                return wrong_argument_quantity;
            }
            if (!(file1 = fopen(argv[2], "r")) || !(file2 = fopen(out, "w+"))) {
                
                fcloseall();
                free(out);
                printf("Ошибка чтения файла\n");
                return file_error;
            }
            find_a(file1, file2);
            printf("Выполнено\n");
            break;
        default:
            printf("Флаг не найден\n");
            free(out);
            return no_flag;
    }
    
    fcloseall();
    free(out);
}