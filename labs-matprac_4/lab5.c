#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef enum errors{
    ok, fail, memory_allocation_problem,
    unknown_error, wrong_brackets, wrong_expression,
    includes_variables, overflow

}errors;

typedef struct Node{
    char * value;
    struct Node * next; 
}Node;

typedef struct operator{
    char symbol;
    int priority;
}operator;

void free_node(Node * ptr){
    free(ptr->value);
    free(ptr);
}

typedef struct Stack{
    Node * top;
    int size;
}Stack;

typedef struct doubleNode{
    double value;
    struct doubleNode * next;
}doubleNode;

typedef struct doubleStack{
    doubleNode * top;
    int size;
}doubleStack;

errors is_double(char * str){
    int n = 0;
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if ((str[i] == '.') && (n == 0) && (i != 0) && (i != (strlen(str) - 1))) n++;
        else if (str[i] < '0' || str[i] > '9') 
            return fail;
    }
    return ok;
}

Stack * create_stack(){
    Stack * stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

doubleStack * create_double_stack(){
    doubleStack * stack = malloc(sizeof(doubleStack));
    if (stack == NULL) return NULL;
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

errors add_el(Stack * stack, char * val){
    Node * ptr = malloc(sizeof(Node));
    if (ptr == NULL) return memory_allocation_problem;
    ptr->value = strdup(val);
    if (ptr->value == NULL) return memory_allocation_problem;
    ptr->next = stack->top;
    stack->top = ptr;
    stack->size++;
    return ok;
}

errors add_double(doubleStack * dstack, double val){
    if (dstack == NULL) return fail;
    doubleNode * ptr = malloc(sizeof(doubleNode));
    if (ptr == NULL) return memory_allocation_problem;
    ptr->value = val;
    ptr->next = dstack->top;
    dstack->top = ptr;
    dstack->size++;
    return ok;
}

void pop(Stack * stack){
    if (stack == NULL) return;
    if (stack->size == 0) return;
    Node * ptr = stack->top;
    if (stack->size == 1){
        free_node(ptr);
        stack->top = NULL;
        stack->size--;
        return;
    }
    stack->top = ptr->next;
    free_node(ptr);
    stack->size--;
}

void pop_double(doubleStack * stack){
    if (stack == NULL) return;
    if (stack->size == 0) return;
    doubleNode * ptr = stack->top;
    if (stack->size == 1){
        free(ptr);
        stack->top = NULL;
        stack->size--;
        return;
    }
    stack->top = ptr->next;
    free(ptr);
    stack->size--;
}

void free_stack(Stack * stack){
    if (stack == NULL) return;
    while(stack->size){
        pop(stack);
    }
    free(stack);
}

void free_double_stack(doubleStack * stack){
    if (stack == NULL) return;
    while(stack->size){
        pop_double(stack);
    }
    free(stack);
}

int priority(char symbol){
    switch (symbol){
        case ')':
            return 0;
        case '(':
            return 0; 
        case '+':
            return 1; 
        case '-':
            return 1; 
        case '*':
            return 2; 
        case '%':
            return 2; 
        case '/':
            return 2; 
        case '^':
            return 3;
        case '~':
            return 4;
        default:
            return -1; 
    }
}

errors is_variable(char * k){
    while (*k){
        if ((*k >= 'A') && (*k <= 'Z')){
            k++;
            continue;
        }
        if ((*k >= 'a') && (*k <= 'z')){
            k++;
            continue;
        }
        if (*k >= '0' && *k <= '9'){
            k++;
            continue;
        }
        return fail;
    }
    return ok;
}

errors is_digit(char k){
    if ((k >= '0') || (k <= '9')) return ok;
    return fail;
}


char * polish_notation(char * str, errors * status_code){
    if (str == NULL) {
        *status_code = wrong_expression;
        return NULL;
    }
    int quantity = 0;
    Stack * stack = create_stack();
    char * answer = malloc(sizeof(char) * (1 + strlen(str)));
    if (answer == NULL){
        free_stack(stack);
        return NULL;
    }
    char var[1 + strlen(str)];
    int num = 0;
    if ((stack == NULL) || (answer == NULL)) {
        free_stack(stack);
        free(answer);
        *status_code = memory_allocation_problem;
        return NULL;
    }
    int i = 0;
    while(*str){
        if (priority(*str) == -1){
            if (*(str - 1) == ')'){
                free_stack(stack);
                free(answer);
                *status_code = wrong_expression;
                return NULL;
            }
            var[num++] = *str;
            if ((priority(answer[i-2]) != -1) && (answer[i-1] == ',')) i--;
            answer[i++] = *str;
        }
        else if(priority(*str) == 0){
            if (*str == '('){
                if (priority(*(str - 1)) == -1 && (i != 0)){
                    free_stack(stack);
                    free(answer);
                    *status_code = wrong_expression;
                    return NULL;
                }
                char tmp[2] = {*str, '\0'};
                if (add_el(stack, tmp) != ok){
                    free(answer);
                    free_stack(stack);
                    *status_code = memory_allocation_problem;
                    return NULL;
                }

                
            }
            else if (*str == ')'){
                if (stack->top == NULL){
                    *status_code = wrong_brackets;
                    free(answer);
                    free_stack(stack);
                    return NULL;
                }
                while (*(stack->top->value) != '('){
                    answer[i++] = *(stack->top->value);
                    pop(stack);
                    if (stack->top == NULL){ 
                        *status_code = wrong_brackets;
                        free(answer);
                        free_stack(stack);
                        return NULL;
                    }
                }
                pop(stack);
            }
            else{ 
                free_stack(stack);
                free(answer);
                *status_code = unknown_error;
                return NULL;
            }
        }
        else {
            var[num] = '\0';
            if (((is_double(var) != ok) && (is_variable(var) != ok)) || (*(str + 1) == '\0')){
                free_stack(stack);
                free(answer);
                *status_code = wrong_expression;
                return NULL;
            }
            num = 0;
            if ((*str == '~') && ((priority(*(str + 1)) != -1) && (priority(*(str + 1) != 0)))){
                free_stack(stack);
                free(answer);
                *status_code = wrong_expression;
                return NULL;
            }
            else if (*str != '~'){
                if ((priority(*(str - 1)) != -1) && (*(str - 1) != ')') || (i == 0)){
                    free_stack(stack);
                    free(answer);
                    *status_code = wrong_expression;
                    return NULL;
                }
            }
            if ((answer[i-1] != ',') && (i != 0))
                answer[i++] = ',';
            if (stack->top != NULL){
                while (priority(*str) <= priority(*(stack->top->value))){
                    while (answer[i-1] == ',') i--;
                    answer[i++] = *stack->top->value;
                    pop(stack);
                    if (stack->size == 0) break;
                }
            }
            char tmp[2] = {*str, '\0'};
            if (add_el(stack, tmp) != ok){
                free(answer);
                free_stack(stack);
                *status_code = memory_allocation_problem;
                return NULL;
            }
        }
        str++;
    }
    var[num] = '\0';
    if ((is_double(var) != ok) && (is_variable(var) != ok)){
        free_stack(stack);
        free(answer);
        *status_code = wrong_expression;
        return NULL;
    }
    while (stack->size){
        if (*(stack->top->value) == '('){ 
            free_stack(stack);
            free(answer);
            *status_code = wrong_brackets;
            return NULL;
        }
        answer[i++] = *(stack->top->value);
        pop(stack);
    }
    answer[i] = '\0';
    free_stack(stack);
    *status_code = ok;
    return answer;
}

double solving(double str1, double str2, char k){
    switch (k){
        case '+':
            return (str1 + str2);
        case '-':
            return (str1 - str2);
        case '/':
            return (double)((int)(str1 / str2));
        case '*':
            return (str1 * str2);
        case '^':
            return pow(str1, str2);
        case '%':
            return str1 - ((int)(str1 / str2) * str2);
        case '~':
            return 0 - str1; 
        default:
            return 0;
    }
}

double solving_polish_notation(char * str, errors * status_code){
    if (str == NULL) {
        *status_code = wrong_expression;
        return 0;
    }
    char * begin = str;
    doubleStack * stack = create_double_stack();
    if (stack == NULL){
        *status_code = memory_allocation_problem;
        return memory_allocation_problem;
    }
    char array[strlen(str)];
    int i = 0;
    int quantity = 0;
    double ans;
    while (*str){
        if (*str == ','){

            array[i] = '\0';
            if (add_double(stack, atof(array)) == memory_allocation_problem){
                free_double_stack(stack);
                *status_code = memory_allocation_problem;
                return memory_allocation_problem;
            }
            i = 0;
        }
        else if (priority(*str) == -1){
            if ((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z')){
                free_double_stack(stack);
                *status_code = includes_variables;
                return includes_variables;
            }
            array[i++] = *str;

        }
        else {
            array[i] = '\0';
            if (i != 0){
                if (add_double(stack, atof(array)) == memory_allocation_problem){
                    free_double_stack(stack);
                    *status_code = memory_allocation_problem;
                    return memory_allocation_problem;
                }  
            }
            i = 0;
            if (*str == '~'){
                ans = solving(stack->top->value, 0, *str);

                pop_double(stack);
                if (add_double(stack, ans) == memory_allocation_problem){
                    free_double_stack(stack);
                    *status_code = memory_allocation_problem;
                    return memory_allocation_problem;
                }
 
            }
            else {
                ans = solving(stack->top->next->value, stack->top->value, *str);
                pop_double(stack);
                pop_double(stack);
                if (add_double(stack, ans) == memory_allocation_problem){
                    free_double_stack(stack);
                    *status_code = memory_allocation_problem;
                    return memory_allocation_problem;
                }
                
            }
        }
        str++;
    }
    double otvet;
    if (stack->top != NULL)
        otvet = stack->top->value;
        else{ 
        free_double_stack(stack);
        return atof(begin);
    }
    free_double_stack(stack);
    *status_code = ok;
    return otvet;
}

void delete_spaces(char * str){
    char array[strlen(str)];
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++){
        if (str[i] != ' ') array[j++] = str[i];
    }
    array[j] = 0;
    strcpy(str, array);
}

int main()
{
    errors status_code = 12321;
    char str[1024] = "((1)*(1)/(1)/(1)*(1)*(1))*34/((()))";
    
    delete_spaces(str);
    printf("%s\n", str);
    char * ans = polish_notation(str, &status_code);
    switch(status_code){
        case ok:
            printf("%s\n", ans);
            break;
        case memory_allocation_problem:
            puts("Ошибка выделения памяти");
            break;
        case wrong_expression:
            puts("Неверно записано выражение");
            break;
        case wrong_brackets:
            puts("Нарушен баланс скобок");
            break;
        case unknown_error:
            puts("Неизвестная ошибка");
            break;
        default:
            puts("Где-то обосрался (жидко)");
            break;
    }
    double solv = solving_polish_notation(ans, &status_code);
    switch (status_code){
        case memory_allocation_problem:
            puts("Ошибка выделения памяти");
            break;
        case includes_variables:
            puts("Выражение содержит переменные и не может быть вычислено");
            break;
        case wrong_expression:
            puts("Неверно записано выражение (вычисление невозможно)");
            break;
        case ok:
            printf("%f\n", solv);
            break;

    }
    free(ans);
    return ok;
}