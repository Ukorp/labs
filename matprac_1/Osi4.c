#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum errors{
    OK, ARGUMENT_ERROR, FILE_ERROR , FAIL
}errors;

typedef enum flags{
    mask, xor8, xor32, no_flag
}flags;

flags define_flag(char * str){
    if (str == NULL) return no_flag;
    if (strcmp(str, "mask") == 0) return mask;
    if (strcmp(str, "xor8") == 0) return xor8;
    if (strcmp(str, "xor32") == 0) return xor32;
    return no_flag;
}

char solve_xor8(FILE * file){
    char result = 0;
    char tmp;
    while ((tmp = getc(file)) != EOF){
        result ^= tmp;
    }
    
    return result;
}

char solve_xor32(FILE * file){
    char result = 0;
    char byte = 1;
    int ans = 0;
    while (byte != EOF){
        int tmp = 0;
        for (int i = 0; i < 4; ++i){
            byte = fgetc(file);
            
            tmp = tmp << 8;
            if (byte != EOF){
                tmp |= byte;
            }
        }
        printf("%d\n", tmp);
        ans ^= tmp;
    }
    
    return ans;
}

int solve_mask (FILE * file, char * hex, errors * status_code){
    if (hex == NULL){
        *status_code = FAIL;
        return FAIL;
    }
    if (file == NULL){
        *status_code = FAIL;
        return FAIL;
    }
    char * endptr;
    int hex_mask = strtol(hex, &endptr, 16);
    if ((*endptr != '\0') || (strlen(hex) == 0)){
        *status_code = FAIL;
        return FAIL;
    }

    char result = 0;
    char byte = 1;
    int ans = 0;
    while (byte != EOF){
        int tmp = 0;
        for (int i = 0; i < 4; ++i){
            byte = fgetc(file);
            tmp = tmp << 8;
            if (byte != EOF){
                tmp |= byte;
            }
        }
        if (tmp == hex_mask) ans++;
    }
    *status_code = OK;
    return ans;
}



int main(int argc, char * argv[]){
    FILE * file1;
    errors status_code;
    char result = 0;
    char tmp;
    if (argc < 3){
        printf("Неверное число аргументов\n");
        return ARGUMENT_ERROR;
    }
    if ((file1 = fopen(argv[1], "r")) == 0){
        printf("Ошибка открытия файла\n");
        return FILE_ERROR;
    }
    flags flag = define_flag(argv[2]);
    switch(flag){
        case xor8:
            printf("%d\n", solve_xor8(file1));
            break;
        case xor32:
            printf("%d\n", solve_xor32(file1));
            break;
        case mask:
            if (argc != 4){
                printf("Неверное число аргументов\n");
                return ARGUMENT_ERROR;
            }
            int otvet = solve_mask(file1, argv[3], &status_code);
            if (status_code == OK)
                printf("%d\n", otvet);
            else puts("ОШИБКААА");
            break;
    }
    printf("%d", result);
    fseek(file1, 0,SEEK_SET);
}