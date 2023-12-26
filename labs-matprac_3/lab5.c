#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum errors{
    ok, fail, memory_allocation_problem,
    overdue, already_took, not_find
}errors;

typedef struct Student{
    unsigned int id;
    char name[50];
    char surname[50];
    char group[50];
    unsigned int * marks;
}Student;


errors isIntChar(char * str){
    
    for (int i = 0; i < strlen(str); i++){
        if (str[i] < '0' || str[i] > '9') 
            return fail;
    }
    return ok;
}

int is_good_word(char * str){
    if (strlen(str) == 0) return fail;
    for (int i = 0; i < strlen(str); ++i){
        if ((str[i] < 'A' || str[i] > 'Z') && (str[i] < 'a' || str[i] > 'z')){
            return fail;
        }
    }
    return ok;
}

int is_usigned_int(char * str){
    for (int i = 0; i < strlen(str); i++){
        if (str[i] < '0' || str[i] > '9') 
            return fail;
    }
    return ok;
}

void free_student(Student * student){
    free(student->marks);
}

Student create_student(FILE * in, errors * status_code){
    Student answer;
    answer.marks = (unsigned int *)malloc(sizeof(unsigned int) * 5);
    if (answer.marks == NULL){
        *status_code = memory_allocation_problem;
        return answer;
    }
    char container[4096];

    fgets(container, 4096, in);
    if (container[strlen(container) - 1] == '\n')
        container[strlen(container) - 1] = ' ';
    char * tmp = strtok(container, " \t");

    if (is_usigned_int(tmp) != ok){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.id = atoi(tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (is_good_word(tmp) != ok){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    strcpy(answer.name, tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (is_good_word(tmp) != ok){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    strcpy(answer.surname, tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    strcpy(answer.group, tmp);
    
    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (atoi(tmp) < 1 || atoi(tmp) > 5){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.marks[0] = atoi(tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (atoi(tmp) < 1 || atoi(tmp) > 5){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.marks[1] = atoi(tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (atoi(tmp) < 1 || atoi(tmp) > 5){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.marks[2] = atoi(tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (atoi(tmp) < 1 || atoi(tmp) > 5){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.marks[3] = atoi(tmp);

    tmp = strtok(NULL, " \t");
    if (tmp == NULL){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    if (atoi(tmp) < 1 || atoi(tmp) > 5){
        *status_code = fail;
        free(answer.marks);
        return answer;
    }
    answer.marks[4] = atoi(tmp);

    *status_code = ok;
    return answer;
}

Student find_student(Student * students, unsigned int id, int size, errors * status_code){
    for (int i = 0; i < size; ++i){
        if (id == students[i].id){
            *status_code = ok;
            return students[i];
        }
    }
    *status_code = not_find;
    return students[0];
}

errors fprintf_student(Student * students, int size, unsigned int id, FILE * out){
    errors status = ok;
    Student good_student;
    good_student = find_student(students, id, size, &status);
    if (status == not_find) return not_find;
    else {
        double average_mark = 0;
        for (int i = 0; i < 5; ++i) average_mark += good_student.marks[i];
        average_mark /= 5.0;
        fprintf(out, "%s %s %s %f\n", good_student.name, good_student.surname, good_student.group,average_mark);
    }
}

void print_students(Student * students, int size, FILE * file){
    for (int i = 0; i < size; i++){
        fprintf(file, "%u %s %s %s %u %u %u %u %u\n",
        students[i].id, students[i].name, 
        students[i].surname, students[i].group,
        students[i].marks[0], students[i].marks[1], 
        students[i].marks[2], students[i].marks[3], 
        students[i].marks[4]);
    }
}

void fill_array(FILE * file, Student ** students_list, int * size){
    errors status_code = ok;
    while (feof(file) == 0){
        (*students_list)[(*size)++] = create_student(file, &status_code);
        if (status_code != ok){
            (*size)--;
            continue;
        }
        Student * ptr = (Student *)realloc(*students_list, sizeof(Student) * (*size + 1));
        if (ptr != NULL) *students_list = ptr;
    }
}

double average_mark(Student student){
    double avg_mark = 0;
    for(int i = 0; i < 5; ++i) avg_mark += student.marks[0];
    return avg_mark / 5.0;
}

double all_average_mark(Student * student_list, int size){
    double avg = 0;
    for (int i = 0; i < size; ++i){
        avg += average_mark(student_list[i]);
    }
    return avg / (double)size;
}

void fprintf_good_student(Student * student_list, int size, FILE * out){
    double avg = all_average_mark(student_list, size);
    for (int i = 0; i < size; ++i){
        if (average_mark(student_list[i]) > avg){
            fprintf(out, "%s %s\n", student_list[i].name, student_list[i].surname);
        }
    }
}

void free_student_list(Student * student, int size){
    for (int i = 0; i < size; i++){
        free_student(&student[i]);
    }
    free(student);
}

void print_menu(){
    puts("\tМеню");
    puts("1 - отсортировать студентов");
    puts("2 - записать информацию о студенте по id");
    puts("3 - записать информацию о студентах, средний балл которых выше среднего по группе");
    puts("4 - выход");
    printf("Введите команду: ");
}

void print_sort(){
    puts("1 - сортировка по id");
    puts("2 - сортировка по имени");
    puts("3 - сортировка по фамилии");
    puts("4 - сортировка по группе");
    puts("5 - fuck go back");
    printf("Введите команду: ");
}

int comp_id(const Student *, const Student *);
int comp_name(const Student *, const Student *);
int comp_surname(const Student *, const Student *);
int comp_group(const Student *, const Student *);

int main(int argc, char * argv[]){
    if (argc != 3){
        puts("Неверное число аргументов\n");
        return fail;
    }
    FILE * in, * out;
    if (strcmp(argv[1], argv[2]) == 0){
        puts("Файлы должны быть разные");
        return fail;
    }
    in = fopen(argv[1], "r+t");
    if (in == NULL){
        puts("Ошибка открытия файла\n");
        return fail;
    }
    out = fopen(argv[2], "w+");
    if (out == NULL){
        puts("Ошибка открытия файла\n");
        return fail;
    }
    errors status_code = ok;
    int size = 0;
    Student * students_list = (Student *)malloc(sizeof(Student));
    fill_array(in, &students_list, &size);
    fclose(in);
    char container[1024];

    while(1){
        print_menu();
        int input_num;
        fgets(container, 1024, stdin);
        container[strlen(container) - 1] = '\0';
        if (isIntChar(container) != ok){
            puts("Команда не найдена\n");
            continue;
        }
        input_num = atoi(container);
        switch (input_num){
            case 1:
                print_sort();
                fgets(container, 1024, stdin);
                container[strlen(container) - 1] = '\0';
                if (isIntChar(container) != ok){
                    puts("Команда не найдена\n");
                    break;
                }
                input_num = atoi(container);
                in = fopen(argv[1], "w+");
                if (in == NULL){
                    puts("Ошибка открытия файла\n");
                    free_student_list(students_list, size);
                    fclose(out);
                    return fail;
                }
                switch (input_num){
                    case 1:
                        qsort(students_list, size, sizeof(Student), (int (*) (const void *, const void *)) comp_id);
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        puts("Успешно!\n");
                        break;
                    case 2:
                        qsort(students_list, size, sizeof(Student), (int (*) (const void *, const void *)) comp_name);
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        puts("Успешно!\n");
                        break;
                    case 3:
                        qsort(students_list, size, sizeof(Student), (int (*) (const void *, const void *)) comp_surname);
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        puts("Успешно!\n");
                        break;
                    case 4:
                        qsort(students_list, size, sizeof(Student), (int (*) (const void *, const void *)) comp_group);
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        puts("Успешно!\n");
                        break;
                    case 5:
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        break;
                    default:
                        print_students(students_list, size, in);
                        fflush(in);
                        fclose(in);
                        puts("Команда не найдена\n");
                        break;
                }
                break;
            case 2:
                unsigned int id = 0;
                puts("Введите id студента: ");
                fgets(container, 1024, stdin);
                container[strlen(container) - 1] = '\0';
                if (isIntChar(container) != ok){
                    puts("Команда не найдена\n");
                    break;
                }
                id = atoi(container);
                if (fprintf_student(students_list, size, id, out) == not_find){
                    puts("Студент не найден");
                    break;
                }
                fflush(out);
                puts("Успешно!");
                break;
            case 3:
                fprintf_good_student(students_list, size, out);
                fflush(out);
                puts("Успешно!");
                break;
            case 4:
                free_student_list(students_list, size);
                puts("До скорых встреч!\n");
                fclose(out);
                return ok;
            default:
                puts("Команда не найдена\n");
                break;

        }
    }
    return ok;
}

int comp_id(const Student * one, const Student * two){
    if (one->id != two->id) return one->id - two->id;
    if (strcmp(one->name, two->name) != 0) return strcmp(one->name, two->name);
    if (strcmp(one->surname, two->surname) != 0) return strcmp(one->surname, two->surname);
    return strcmp(one->group, two->group);
}

int comp_name(const Student * one, const Student * two){
    if (strcmp(one->name, two->name) != 0) return strcmp(one->name, two->name);
    if (strcmp(one->surname, two->surname) != 0) return strcmp(one->surname, two->surname);
    if (one->id != two->id) return one->id - two->id;
    
    
    return strcmp(one->group, two->group);
}

int comp_surname(const Student * one, const Student * two){
    
    if (strcmp(one->surname, two->surname) != 0) return strcmp(one->surname, two->surname);
    if (strcmp(one->name, two->name) != 0) return strcmp(one->name, two->name);
    if (one->id != two->id) return one->id - two->id;
    return strcmp(one->group, two->group);
}

int comp_group(const Student * one, const Student * two){
    if (strcmp(one->group, two->group) != 0) return strcmp(one->group, two->group);
    if (strcmp(one->name, two->name) != 0) return strcmp(one->name, two->name);
    if (strcmp(one->surname, two->surname) != 0) return strcmp(one->surname, two->surname);
    return one->id - two->id;
}