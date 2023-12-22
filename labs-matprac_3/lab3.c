#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define eps 0.00000000000001

typedef enum errors{
    ok, overflow, memory_allocation_problem, data_error,
    arguments_error, file_open_error

}errors;

typedef enum flags{
    a, d, no_flag
}flags;

typedef struct Employee{
    int id;
    char * name;
    char * surname;
    double salary;
}Employee;

flags define_flag(char * str){
    if (strlen(str) != 2) return no_flag;
    if (str[0] != '/' && str[0] != '-') return no_flag;
    if (str[1] == 'a') return a;
    if (str[1] == 'd') return d;
    return no_flag;
}

int is_unsigned_int(char * str){
    for (int i = 0; i < strlen(str); i++){
        if (str[i] < '0' || str[i] > '9') 
            return data_error;
    }
    return ok;
}

int is_name(char * str){
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z'))
            return data_error;
        if ((str[i] >= 'a' && str[i] <= 'z') && (i == 0)) str[i] = str[i] - 'a' + 'A';
        else if ((str[i] >= 'A' && str[i] <= 'Z') && (i != 0)) str[i] = str[i] - 'A' + 'a';
    }
    return ok;
}

int is_double(char * str){
    int n = 0;
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '.') && (n == 0) && (i != 0) && (i != (strlen(str) - 1))) n++;
        else if (str[i] < '0' || str[i] > '9') 
            return data_error;
    }
    return ok;
}

int better_string(char * str1, char * str2){
    int size1 = strlen(str1);
    int size2 = strlen(str2);
    int size = (size1 < size2) ? size1 : size2;
    for (int i = 0; i < size; i++){
        if (str1[i] > str2[i]) return 1;
        if (str1[i] < str2[i]) return -1;
    }
    if (size1 > size2) return 1;
    if (size1 < size2) return -1;
    return 0;
}

errors make_id(Employee * array, char * str){
    if (is_unsigned_int(str) == data_error){
        return data_error;
    }
    array->id = atoi(str);
    return ok;
}

errors make_name(Employee * array, char * str){
    if (is_name(str) == data_error){
        return data_error;
    }
    array->name = (char *)malloc(sizeof(char) * strlen(str));
    if (array->name == NULL) return memory_allocation_problem; 
    strcpy(array->name, str);
    return ok;
}
errors make_surname(Employee * array, char * str){
    if (is_name(str) == data_error){
        return data_error;
    }
    array->surname = (char *)malloc(sizeof(char) * strlen(str));
    if (array->surname == NULL) return memory_allocation_problem;
    strcpy(array->surname, str);
    return ok;
}

errors make_salary(Employee * array, char * str){
    if (is_double(str) == data_error){
        return data_error;
    }
    array->salary = atof(str);
    
    return ok;
}

errors line_to_string(char * str, Employee * array){
    int size = strlen(str);
    int counter = 0;
    char * tmp = (char *)malloc(sizeof(char) * size);
    int j = 0;
    array->id = -1;
    array->salary = -1;
    for (int i = 0; i < size; ++i){
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\0'){
            if (str[i - 1] == ' ' || str[i - 1] == '\t') continue;
            switch (counter){
                case 0:
                    tmp[j] = '\0';
                    make_id(array, tmp);
                    j = 0;
                    break;
                case 1:
                    tmp[j] = '\0';
                    if (make_name(array, tmp) == memory_allocation_problem){
                        puts("Memory allocation problem\n");
                        return memory_allocation_problem;
                    };
                    j = 0;
                    break;
                case 2:
                    tmp[j] = '\0';
                    if (make_surname(array, tmp) == memory_allocation_problem){
                        puts("Memory allocation problem\n");
                        return memory_allocation_problem;
                    };
                    j = 0;
                    break;
                case 3:
                    tmp[j] = '\0';
                    make_salary(array, tmp);
                    j = 0;
                    break;
                    
                default:
                    return data_error;
            }
            counter++;
            continue;
        }
        else if (counter == 4) return data_error;
        tmp[j++] = str[i];
    }
    if (counter == 3){
        tmp[j] = '\0';
        make_salary(array, tmp);
        j = 0;
        counter++;
    }
    if ((array->id == -1) || (array->salary == -1) || (array->name == NULL) || (array->name == NULL)) return data_error;
    return ok;

}

void free_all(char * str, Employee * array, int i){
    free(str);
    for (i; i >= 0; i--){
        free(array[i].name);
        free(array[i].surname);
    }
}

int comp_d (const Employee *, const Employee *);
int comp_a (const Employee *, const Employee *);

int main(int argc, char * argv[]){
    if (argc != 3){
        printf("Неверное число аргументов\n");
        return arguments_error;
    }
    flags defined = define_flag(argv[1]);
    if ((defined != a) && (defined != d)){
        printf("Флаг не найден\n");
        return no_flag;
    }
    FILE * file;
    file = fopen(argv[2], "r");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return file_open_error;
    }
    char * str = (char *)malloc(sizeof(char) * 4096);
    if (str == NULL){
        printf("Ошибка выделения памяти\n");
        return memory_allocation_problem;
    }
    Employee * array = (Employee *)malloc(sizeof(Employee));
    if (array == NULL){
        printf("Ошибка выделения памяти\n");
        free(str);
        return memory_allocation_problem;
    }
    int i = 0;
    while(fgets(str, 4096, file)){
        Employee * ptr = (Employee *)realloc(array, sizeof(Employee) * (i + 1));
        if (ptr == NULL){
            printf("Ошибка перевыделения памяти\n");
            free_all(str, array, i);
            return memory_allocation_problem;
        }
        array = ptr;
        if (str[0] == '\n') continue;
        if(line_to_string(str, &array[i++]) == data_error){
            printf("Ошибка данных\n");
            free_all(str, array, i-1);
            return data_error;
        };
    }
    switch(defined){
        case a:
            qsort(array, i, sizeof(Employee), (int (*)(const void *, const void *))comp_a);
            break;
        case d:
            qsort(array, i, sizeof(Employee), (int (*)(const void *, const void *))comp_d);
            break;
            
    }
    
    for (int j = 0; j < i; j++){
        printf("%d %s %s %f\n", array[j].id, array[j].name, array[j].surname, array[j].salary);
    }
    free_all(str, array, i-1);
    return 0;
}

int comp_a(const Employee * first, const Employee * second){
    if ((fabs(first->salary - second->salary)) > eps) return first->salary > second->salary;
    else if (better_string(first->surname, second->surname) != 0) return better_string(first->surname, second->surname);
    else if (better_string(first->name, second->name) != 0) return better_string(first->name, second->name);
    else first->id > second->id;

}

int comp_d(const Employee * first, const Employee * second){
    if ((fabs(first->salary - second->salary)) > eps) return first->salary < second->salary;
    else if (better_string(first->surname, second->surname) != 0) return better_string(second->surname, first->surname);
    else if (better_string(first->name, second->name) != 0) return better_string(second->name, first->name);
    else first->id < second->id;

}