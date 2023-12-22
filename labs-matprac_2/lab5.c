#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum errors {
    success,
    invalid_parameter,
    error_alloc
} errors;

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

errors to_roman(int number, char** r_value) {
    const int r_numbers[13] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    const char r_letters[13][2] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
    int r_size = 0;
    int r_capacity = 2;
    (*r_value) = (char*)malloc(sizeof(char) * r_capacity);
    if (*r_value == NULL) {
        return error_alloc;
    }
    if (number < 0) {
        (*r_value)[0] = '-';
        r_size++;
        number = abs(number);
    }
    //printf("%d--sd\n", number);
    while (number) {
        int index = 13;
        while (r_numbers[--index] > number);
        for (int i = 0; i < 2; i++) {
            if (r_letters[index][i] == 'I' || r_letters[index][i] == 'V' || r_letters[index][i] == 'X' || 
            r_letters[index][i] == 'L' || r_letters[index][i] == 'C' || r_letters[index][i] == 'D' || r_letters[index][i] == 'M') {
                (*r_value)[r_size] = r_letters[index][i];
                r_size++;
                if (r_size == r_capacity - 1) {
                    r_capacity *= 2;
                    status_realloc st_realloc = my_realloc(r_value, r_capacity);
                    if (st_realloc == status_realloc_fail) {
                        return error_alloc;
                    }
                }
            }
        }
        number -= r_numbers[index];
    }
    (*r_value)[r_size] = '\0';
    return success;
}
void fibonacci(unsigned int result[2], unsigned int number) {
    unsigned int count = 0;
    unsigned int tmp1 = 0, tmp2 = 1;
    unsigned int next = tmp1 + tmp2;
    while (next <= number) {
        tmp1 = tmp2;
        tmp2 = next;
        next = tmp1 + tmp2;
        count++;
    }
    result[0] = count;
    result[1] = tmp2;
}


void zeckendorf(int* result_size, unsigned int number, unsigned int result[number][2]) {
    (*result_size) = 0; 
    while (number) {
        fibonacci(result[*result_size], number);
        (*result_size)++;
        number -= result[(*result_size) - 1][1];
    }
}

errors convert_from_decimal(long long int decimal_number, int base, int flag,  char** result) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    int index = 0;
    bool is_minus = false;
    (*result) = (char*)malloc((32 + 1) * sizeof(char));
    if(*result == NULL) {
        return error_alloc;
    }
    int size = 0;
    char tmp_res[32];
    if (decimal_number == 0) {
        (*result)[index] = '0';
        (*result)[index + 1] = '\0';
        return success;
    }
    if (decimal_number < 0) {
        is_minus = true;
        (*result)[index++] = '-';
        size++;
    }
    decimal_number = abs(decimal_number);
    while (decimal_number) {
        long long int remainder = decimal_number % base;
        if (remainder < 10) {
            (*result)[index++] = remainder + '0';
        } else {
            if (flag) {
                (*result)[index++] = remainder - 10 + 'A';
            } else {
                (*result)[index++] = remainder - 10 + 'a';
            }
        }
        size++;
        if (size > sizeof(*result) - 1) {
            status_realloc st_realloc = my_realloc(result, size * 2);
            if (st_realloc == status_realloc_fail) {
                return error_alloc;
            }
        }
        decimal_number /= base;
    }
    (*result)[size] = '\0';
    int length = strlen(*result);
    for (int i = 0, j = 0; length - 1 - j > i; i++, j++) {
        if (is_minus) {
            i++;
            is_minus = false;
        }
        char tmp = (*result)[i];
        (*result)[i] = (*result)[length - 1 - j];
        (*result)[length - 1 - j] = tmp;
    }
    return success;
}

errors convert_to_decimal(char* number_str, int base, long long int * dec_number, int flag) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    unsigned long long int decimal_number = 0;
    int power = strlen(number_str) - 1;  
    bool start = true;
    for (int i = 0; i < strlen(number_str); i++) {
        char digit = number_str[i];
        if (digit == '-' && start) {
            if (start) {
                power--;
                start = false;
                continue;
            }
        }
        int digit_value;
        if (isdigit(digit)) {
            digit_value = digit - '0';
        } else if (isalpha(digit)) {
            if (flag) {
                digit_value = digit - 'A' + 10;
            } else {
                digit_value = digit - 'a' + 10;
            }
        } else {
            return invalid_parameter;
        }
        decimal_number += digit_value * pow(base, power);
        power--;
        start = false;
    }
    if (decimal_number > INT_MAX) {
        return invalid_parameter;
    }
    *dec_number = decimal_number;
    return success;
}

bool is_lower(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) {
            return false;
        }
    }
    return true;
} 

bool is_upper(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if ((str[i] >= 'a') && (str[i] <= 'z')) {
            return false;
        }
    }
    return true;
} 

bool is_digit_str(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int overfprintf(FILE * stream, char * format, ...) {
    if (!stream) {
        return -1;
    }
    int size_format = strlen(format);
    if (!size_format) {
        return - 1;
    }
    int argc = 0;
    for (int i = 0; i < size_format; i++) {
        if (format[i] == '%') {
            if (format[i + 1] != '%') {
                argc++;
            }
        }
    }
    va_list ptr;
    va_start(ptr, format);
    int index = 0;
    char* flag = NULL;
    int ret_value = 0;
    for (int i = 0; i < argc; i++) {
        int size_flag = 1;
        int capacity_flag = 2;
        while(format[index] != '%' && index < size_format) {
            ret_value += fprintf(stream, "%c", format[index]);
            index++;
        }
        flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
        if (!flag) {
            return -1;
        }
        flag[0] = '%';
        index++;
        while ((isalpha(format[index]) || isdigit(format[index]) || (format[index] == '%' && format[index - 1] == '%') ||
        (format[index] == '.' && (isalpha(format[index + 1]) || isdigit(format[index + 1]) || format[index + 1] == '*'))
        || (format[index] == '#') || (format[index] == '+') || (format[index] == '-') || (format[index] == '0')) && (index < size_format))
        {
            char symbol = format[index];
            flag[size_flag] = symbol;
            size_flag++;
            if (size_flag == capacity_flag - 1) {
                capacity_flag *= 2;
                status_realloc st_realloc = my_realloc(flag, capacity_flag);
                if (st_realloc == status_realloc_fail) {
                    free(flag);
                    printf("Can`t realloc memory!\n");
                    return -1;
                }
            }
            index++;

        }
        flag[size_flag] = '\0';
        if (!strcmp(flag, "%Ro")) {
            char* roman_value = NULL;
            errors st_roman = to_roman(va_arg(ptr, int), &roman_value);
            if (st_roman == error_alloc) {
                printf("Error alloc!!!\n");
                free(roman_value);
                return -2;
            }
            ret_value += fprintf(stream, "%s", roman_value);
            free(roman_value);
        } 
        else if (!strcmp(flag, "%Zr")) {
            unsigned int number = va_arg(ptr, unsigned int);
            //printf("%d---\n", number);
            int res_size;
            unsigned int res[number][2];
            zeckendorf(&res_size, number, res);
            int count = 1;
            int j = res_size - 1;
            while (j >= 0) {
                
                if (count == res[j][0]) {
                    ret_value += fprintf(stream, "1");
                    j--;
                } else {
                    ret_value += fprintf(stream, "0");
                }
                //printf("%d----\n", i);
                count++;
            }
            ret_value += fprintf(stream, "1");
        } 
        else if (!strcmp(flag, "%Cv")) {
            long long int number_to_convert = va_arg(ptr, long long int);
            argc++;
            int base = va_arg(ptr, int);
            char* result = NULL;
            errors st_convert = convert_from_decimal(number_to_convert, base, 0, &result);
            if (st_convert == error_alloc) {
                free(result);
                printf("Error alloc!!!\n");
                break;
            }
            ret_value += fprintf(stream, "%s",  result);
            free(result);
        } 
        else if (!strcmp(flag, "%CV")) {
            long long int number_to_convert = va_arg(ptr, long long int);
            argc++;
            int base = va_arg(ptr, int);
            char* result = NULL;
            errors st_convert = convert_from_decimal(number_to_convert, base, 1, &result);
            if (st_convert == error_alloc) {
                free(result);
                printf("Error alloc!!!\n");
                break;
            }
            ret_value += fprintf(stream, "%s",  result);
            free(result);
        } 
        else if (!strcmp(flag, "%to")) {
            char* num_to_convert = va_arg(ptr, char*);
            int size = strlen(num_to_convert);
            argc++;
            int base = va_arg(ptr, int);
            long long int result;
            
            if (!is_lower(num_to_convert, size)) {
                printf("Letters are in upper case\n");
                break;
            }
            
            //printf("%s -- %d\n", num_to_convert, base);
            errors st_convert = convert_to_decimal(num_to_convert, base, &result, 0);
            if (st_convert == invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                break;
            }
            ret_value += fprintf(stream, "%lld",  result);
        } 
        else if (!strcmp(flag, "%TO")) {
            char* num_to_convert = va_arg(ptr, char*);
            int size = strlen(num_to_convert);
            argc++;
            int base = va_arg(ptr, int);
            long long int result;
            
            if (!is_upper(num_to_convert, size)) {
                printf("Letters are in lower case\n");
                break;
            }
            
            errors st_convert = convert_to_decimal(num_to_convert, base, &result, 1);
            if (st_convert == invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                break;
            }
            ret_value += fprintf(stream, "%lld",  result);
        } 
        else if (!strcmp(flag, "%mi")) {
            int val = va_arg(ptr, int);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(int); j++) {
                ret_value += fprintf(stream, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%mu")) {
            unsigned int val = va_arg(ptr, unsigned int);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(unsigned int); j++) {
                ret_value += fprintf(stream, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%md")) {
            double val = va_arg(ptr, double);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(double); j++) {
                ret_value += fprintf(stream, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%mf")) {
            float val = va_arg(ptr, double);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(float); j++) {
                ret_value += fprintf(stream, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else {
            ret_value += vfprintf(stream, flag, ptr);
        }
        free(flag);
        flag = NULL;
    }
    if (flag) {
        free(flag);
    }
    va_end(ptr);
    return ret_value;

}

int oversprintf(char* buf, char * format, ...) {
    int size_format = strlen(format);
    if (!size_format) {
        return - 1;
    }
    int argc = 0;
    for (int i = 0; i < size_format; i++) {
        if (format[i] == '%') {
            if (format[i + 1] != '%') {
                argc++;
            }
        }
    }
    va_list ptr;
    va_start(ptr, argc);
    int index = 0;
    char* flag = NULL;
    int ret_value = 0;
    for (int i = 0; i < argc; i++) {
        int size_flag = 1;
        int capacity_flag = 2;
        while(format[index] != '%' && index < size_format) {
            ret_value += sprintf(buf + ret_value, "%c", format[index]);
            index++;
        }
        flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
        if (!flag) {
            return -1;
        }
        flag[0] = '%';
        index++;
        while ((isalpha(format[index]) || isdigit(format[index]) || (format[index] == '%' && format[index - 1] == '%') ||
        (format[index] == '.' && (isalpha(format[index + 1]) || isdigit(format[index + 1]) || format[index + 1] == '*'))
        || (format[index] == '#') || (format[index] == '+') || (format[index] == '-') || (format[index] == '0')) && (index < size_format))
        {
            char symbol = format[index];
            flag[size_flag] = symbol;
            size_flag++;
            if (size_flag == capacity_flag - 1) {
                capacity_flag *= 2;
                status_realloc st_realloc = my_realloc(flag, capacity_flag);
                if (st_realloc == status_realloc_fail) {
                    free(flag);
                    printf("Can`t realloc memory!\n");
                    return -1;
                }
            }
            index++;

        }
        flag[size_flag] = '\0';
        //printf("%s--\n", flag);
        if (!strcmp(flag, "%Ro\0")) {
            char* roman_value = NULL;
            errors st_roman = to_roman(va_arg(ptr, int), &roman_value);
            if (st_roman == error_alloc) {
                printf("Error alloc!!!\n");
                free(roman_value);
                return -2;
            }
            ret_value += sprintf(buf + ret_value, "%s", roman_value);
            free(roman_value);
        } 
        else if (!strcmp(flag, "%Zr\0")) {
            unsigned int number = va_arg(ptr, unsigned int);
            //printf("%d---\n", number);
            int res_size;
            unsigned int res[number][2];
            zeckendorf(&res_size, number, res);
            int count = 1;
            int j = res_size - 1;
            while (j >= 0) {
                
                if (count == res[j][0]) {
                    ret_value += sprintf(buf + ret_value, "1");
                    j--;
                } else {
                    ret_value += sprintf(buf + ret_value, "0");
                }
                //printf("%d----\n", i);
                count++;
            }
            ret_value += sprintf(buf + ret_value, "1");
        } 
        else if (!strcmp(flag, "%Cv\0")) {
            long long int number_to_convert = va_arg(ptr, long long int);\
            argc++;
            int base = va_arg(ptr, int);
            char* result = NULL;
            errors st_convert = convert_from_decimal(number_to_convert, base, 0, &result);
            if (st_convert == error_alloc) {
                free(result);
                printf("Error alloc!!!\n");
                break;
            }
            ret_value += sprintf(buf + ret_value, "%s",  result);
            free(result);
        } 
        else if (!strcmp(flag, "%CV\0")) {
            long long int number_to_convert = va_arg(ptr, long long int);
            int base = va_arg(ptr, int);
            char* result = NULL;
            argc++;
            errors st_convert = convert_from_decimal(number_to_convert, base, 1, &result);
            if (st_convert == error_alloc) {
                free(result);
                printf("Error alloc!!!\n");
                break;
            }
            ret_value += sprintf(buf + ret_value, "%s",  result);
            free(result);
        } 
        else if (!strcmp(flag, "%to\0")) {
            char* num_to_convert = va_arg(ptr, char*);
            int size = strlen(num_to_convert);
            argc++;
            int base = va_arg(ptr, int);
            long long int result;
            
            if (!is_lower(num_to_convert, size)) {
                printf("Letters are in upper case\n");
                break;
            }
            
            //printf("%s -- %d\n", num_to_convert, base);
            errors st_convert = convert_to_decimal(num_to_convert, base, &result, 0);
            if (st_convert == invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                break;
            }
            ret_value += sprintf(buf + ret_value, "%lld",  result);
        } 
        else if (!strcmp(flag, "%TO\0")) {
            char* num_to_convert = va_arg(ptr, char*);
            int size = strlen(num_to_convert);
            argc++;
            int base = va_arg(ptr, int);
            long long int result;
            
            if (!is_upper(num_to_convert, size)) {
                printf("Letters are in lower case\n");
                break;
            }
            
            errors st_convert = convert_to_decimal(num_to_convert, base, &result, 1);
            if (st_convert == invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                break;
            }
            ret_value += sprintf(buf + ret_value, "%lld",  result);
        } 
        else if (!strcmp(flag, "%mi\0")) {
            int val = va_arg(ptr, int);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(int); j++) {
                ret_value += sprintf(buf + ret_value, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%mu\0")) {
            unsigned int val = va_arg(ptr, unsigned int);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(unsigned int); j++) {
                ret_value += sprintf(buf + ret_value, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%md\0")) {
            double val = va_arg(ptr, double);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(double); j++) {
                ret_value += sprintf(buf + ret_value, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else if (!strcmp(flag, "%mf\0")) {
            float val = va_arg(ptr, double);
            unsigned char* tmp_ptr = (unsigned char*)&val;
            for (int j = 0; j < sizeof(float); j++) {
                ret_value += sprintf(buf + ret_value, "%u", *tmp_ptr % 2);
                tmp_ptr++;
            }
        } 
        else {
            ret_value += vsprintf(buf + ret_value, flag, ptr);
        }
        free(flag);
        flag = NULL;
    }
    if (flag) {
        free(flag);
    }
    va_end(ptr);
    return ret_value;

}

int main(int argc, char* argv[]) {
    FILE* in = fopen("tmp.txt", "w");
    overfprintf(stdout, "wow number is: %Ro, %Cv\n", 5435, 1234, 16);
    overfprintf(stdout, "sfdsdf %to %TO %d%d\n", "abcdef", 16, "10", 10, 12, 12);
    overfprintf(stdout, "%mi %mu %md, %mf \n", (int)10, (unsigned int)12, (double)2.2, (double)2.3);
    overfprintf(stdout, "fsdwow number is: %Zr \n", (unsigned int)10);
    overfprintf(in, "fsdwow number is: %Zk\n", (unsigned int)10);
    overfprintf(stdout, "\n%Ro %CV %Ro %CV %CV\n", 15, 10, 2, 15, 22, 2, 17, 2);
    char* buf = (char*)malloc(sizeof(char) * 32);
    printf("\n\n");
    oversprintf(buf, "wow number is: %Ro, %Cv\n", 5435, 1234, 16);
    printf("%s\n", buf);
    oversprintf(buf, "sfdsdf %to %TO %d%d\n", "abcdef", 16, "10", 10, 12, 12);
    printf("%s\n", buf);
    oversprintf(buf, "%mi %mu fsd ad fsd %md, %mf\n", (int)10, (unsigned int)12, (double)2.2, (double)2.3);
    printf("%s\n", buf);
    oversprintf(buf, "fsdwow number is: %Zr\n", (unsigned int)10);
    printf("%s\n", buf);
    free(buf);
    fclose(in);
}