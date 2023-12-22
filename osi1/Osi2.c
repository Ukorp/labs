#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char login[150];
    int pin;
    int logged;
    int sanctions;
    int uses;
} user;

typedef enum autorization{
    ok, login_fail, password_fail, wrong_password,
    login_already_used, no_user, wrong_int,
    memory_allocation_problem, input_fail,
    bad_login, length_error
}autorization;

typedef enum commands{
    Time, Date, Howmuch, Logout, Sanctions,
    unknown_command
}commands;

typedef enum flags{
    s, m, h, y, unknown_flag
}flags;

flags define_flag(char * str){
    if (strcmp(str, "-s") == 0) return s;
    if (strcmp(str, "-m") == 0) return m;
    if (strcmp(str, "-h") == 0) return h;
    if (strcmp(str, "-y") == 0) return y;
    return unknown_flag;
}

autorization is_int(char * str){
    
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return wrong_int;
    }
    return ok;
}

autorization is_true_login(char * str){
    if ((str == NULL) || (strlen(str) > 6)) return bad_login;
    while (*str){
        if (((*str >= 'A') && (*str <= 'Z')) || ((*str >= 'a') && (*str <= 'z')) || ((*str >= '0') && (*str <= '9'))){
            str++;
            continue;
        }
        else return bad_login;
    }
    return ok;
}

autorization registration(char * login, int pin, user * base, int * number){
    user tmp;
    tmp.logged = 0;
    tmp.sanctions = 200;
    tmp.uses = 0;
    if (strlen(login) > 6) return length_error;
    if (is_true_login(login) != ok) return bad_login;
    for (int i = 0; i < *number; i++){
        if (strcmp(base[i].login, login) == 0) return login_already_used;
    }
    strcpy(tmp.login, login);
    if ((pin < 0) || (pin > 100000)) return password_fail;
    tmp.pin = pin;
    base[*number] = tmp;
    (*number)++;
    return ok;
}

autorization command_limit(user * base, char * login, int num, int number){
    for (int i = 0; i < number; i++){
        if (strcmp(base[i].login, login) == 0){
            base[i].sanctions = num;
            return ok;
        }
    }
    return no_user;
}

autorization log_in(char * login, int pin, user * base, int * number, user ** logged_user){
    user tmp;
    if (*number == 0) return login_fail;
    if ((pin < 0) || (pin > 100000)) return password_fail;
    for (int i = 0; i < *number; i++){
        if ((strcmp(base[i].login, login) == 0) && (base[i].pin == pin)){
            base[i].logged = 1;
            *logged_user = (base + i);
            (*logged_user)->logged = 1;
            
            return ok;
        }
    }
    return login_fail;
}

void print_cmd(){
    puts("Команды:");
    puts("Вывод премени на экран - Time");
    puts("Вывод даты на экран - Date");
    puts("Вывод прошедшего времени  с момента указанной даты - Howmuch <dd:mm:yyyy> flag (-s в секундах, -m в минутах, -h в часах)");
    puts("Применить санкции на пользователя - Sanctions username <number>");
    puts("Выйти из учётной записи - Logout");
}

commands define_command(char * str){
    if (strcmp(str, "Time") == 0) return Time;
    if (strcmp(str, "Date") == 0) return Date;
    if (strcmp(str, "Howmuch") == 0) return Howmuch;
    if (strcmp(str, "Logout") == 0) return Logout;
    if (strcmp(str, "Sanctions") == 0) return Sanctions;
    return unknown_command;
}

void print_menu_autorization(){
    printf("\tМеню авторизации\n");
    printf("\tЕсли вы хотите войти в свою учётною запись - введите 1\n");
    printf("\tЕсли вы хотите зарегистрироваться - введите 2\n");
    printf("\tЕсли вы хотите завершить работу программы - введите 3\n");
}

void scan(char * out){
    char k;
    int i = 0;
    while ((k = getchar()) != '\n'){
        out[i] = k;
        i++;
    }
    out[i] = 0;
}

void delete_space(char * str){
    int i = strlen(str) - 1;
    while (str[i] == ' ' || str[i] == '\n') i--;
    str[i + 1] = 0;
}

autorization is_space(char * str){
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == ' ') return login_fail;
    }
    return ok;
}

int main() {
    user tmp;
    tmp.logged = 0;
    user * logged_user;
    logged_user = &tmp;
    int entry;
    char k[128];
    int size = 10;
    char log[150];
    int pin;
    int i = 0;
    char strochka[150];
    char sep[] = " ";
    user * base = (user *)malloc(sizeof(user) * size);
    if (base == NULL){
        printf("MEMORY ALLOCATION RETURN\n");
        return memory_allocation_problem;
    }
    while (1){
        while(logged_user->logged == 0){
            print_menu_autorization();
            if (fgets(k, 128, stdin) == NULL){
                printf("Ошибка ввода\n");
                free(base);
                return input_fail;
            };
            delete_space(k);
            if (strlen(k) != 1) {
                printf("Введена неверная команда\n");
                continue;
            }
            if ((k[0] != '1') && (k[0] != '2') && (k[0] != '3')){
                printf("Введена неверная команда\n");
                continue;
            }
            entry = k[0] - '0';
            if (entry == 3){
                printf("See you next time!\n");
                free(base);
                return ok;
            }
            printf("Введите логин: ");

            if (fgets(log, 128, stdin) == NULL){
                printf("Ошибка ввода\n");
                free(base);
                return input_fail;
            };
            delete_space(log);
            if (is_space(log)){
                printf("Неправильно введён логин\n");
                continue;
            }
            printf("Введите пин: ");
            if (fgets(strochka, 128, stdin) == NULL){
                printf("ОШИБКА\n");
                free(base);
                return 1;
            };
            delete_space(strochka);
            if (is_int(strochka)){
                printf("Неправильно введён пин\n");
                continue;
            }
            pin = atoi(strochka);

            switch (entry){
                case 1:
                    switch (log_in(log, pin, base, &i, &logged_user)){
                        case password_fail:
                            printf("Неверный символы в пароле\n");
                            break;
                        case login_fail:
                            printf("Неверный логин или пароль\n");
                            break;
                        case ok:
                            printf("Вход совершён\n");
                            break;
                    }
                    break;
                case 2:
                    switch (registration(log, pin, base, &i)){
                        case login_already_used:
                            printf("Логин уже используется\n");
                            break;
                        case password_fail:
                            printf("Неверные символы в пароле/неверная длина пароля\n");
                            break;
                        case bad_login:
                            printf("В логине используются недопустимые символы\n");
                            break;
                        case length_error:
                            printf("Превышено максимальное число символов логина\n");
                            break;
                        case ok:
                            printf("Пользователь успешно зарегистрирован!\n");
                            break;
                    }
                    break;
                case 3:
                    printf("See you next time!\n");
                    free(base);
                    return ok;
                default:
                    printf("Команда не найдена\n");
            }
        }
        char * command = (char *)malloc(sizeof(char) * 128);
        if (command == NULL){
            printf("MEMORY ALLOCATION ERROR\n");
            free(base);
            return memory_allocation_problem;
        }
        char * str;
        int tmp1;
        char sep[] = " :";
        time_t t;
        struct tm * aTm;

        while (logged_user->logged == 1){
            print_cmd();
            if (fgets(command, 128, stdin) == NULL){
                printf("Ошибка считывание строки\n");
                free(base);
                free(command);
                return input_fail;
            }
            
            str = strtok(command, sep);
            delete_space(str);
            switch (define_command(str)){
                case Time:
                    if (logged_user->sanctions <= logged_user->uses){
                        printf("Превышено число операций для данного пользователя [%d]\n", logged_user->sanctions);
                        break;
                    }
                    if (strtok(NULL, sep) != NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    t = time(NULL);
                    aTm = localtime(&t);
                    printf("%02d:%02d:%02d\n", aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
                    logged_user->uses += 1;
                    break;
                case Date:
                    if (logged_user->sanctions <= logged_user->uses){
                        printf("Превышено число операций для данного пользователя [%d]\n", logged_user->sanctions);
                        break;
                    }
                    if (strtok(NULL, sep) != NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    t = time(NULL);
                    aTm = localtime(&t);
                    printf("%02d:%02d:%02d\n", aTm->tm_mday, aTm->tm_mon + 1, aTm->tm_year + 1900);
                    logged_user->uses += 1;
                    break;
                case Howmuch:
                    if (logged_user->sanctions <= logged_user->uses){
                        printf("Превышено число операций для данного пользователя [%d]\n", logged_user->sanctions);
                        break;
                    }
                    t = time(NULL);
                    aTm = localtime(&t);
                    if ((str = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    delete_space(str);
                    if (is_int(str) != ok){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    aTm->tm_mday = atoi(str);

                    if ((str = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    delete_space(str);
                    if (is_int(str) != ok){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    aTm->tm_mon = atoi(str) - 1;

                    if ((str = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    delete_space(str);
                    if (is_int(str) != ok){
                        printf("Неправильно введена дата\n");
                        break;
                    }
                    aTm->tm_year = atoi(str) - 1900;

                    if ((str = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    delete_space(str);
                    int flag = 0;
                    switch (define_flag(str)){
                        case s:
                            
                            printf("%ld\n", (long int)difftime(t, mktime(aTm)));
                            break;
                        case m:
                            printf("%ld\n", (long int)difftime(t, mktime(aTm)) / 60);
                            break;
                        case h:
                            printf("%ld\n", (long int)difftime(t, mktime(aTm)) / 60 / 60);
                            break;
                        case y:
                            printf("%ld\n", (long int)(123 - aTm->tm_year));
                            break;
                        default:
                            printf("Флаг не найден\n");
                            flag = 1;
                            break;
                    }
                    if (!flag){
                        break;
                    }

                    if (strtok(NULL, sep) != NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }

                    logged_user->uses += 1;
                    break;
                case Sanctions:
                    if (logged_user->sanctions <= logged_user->uses){
                        printf("Превышено число операций для данного пользователя [%d]\n", logged_user->sanctions);
                        break;
                    }
                    char * chel;
                    char * chislo;
                    if ((chel = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    delete_space(chel);
                    if ((chislo = strtok(NULL, sep)) == NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    if (strtok(NULL, sep) != NULL){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    delete_space(chislo);
                    if (is_int(chislo) != ok){
                        printf("Неправильно введена команда\n");
                        break;
                    }
                    tmp1 = atoi(chislo);
                    if (command_limit(base, chel, tmp1, i) != ok){
                        printf("Пользователь не найден\n");
                        break;
                    }
                    printf("Введите код для подтверждения: ");
                    if (!fgets(command, 128, stdin)){
                        printf("Ошибка\n");
                        break;
                    }
                    delete_space(command);
                    if (strcmp(command, "12345") != 0){
                        printf("Неправильно введен код\n");
                        break;
                    }

                    logged_user->uses += 1;
                    printf("ok\n");
                    break;
                case Logout:
                    logged_user->logged = 0;
                    printf("Вы вышли из своей учётной записи\n");
                    break;
                default:
                    printf("Команда не найдена\n");
                    break;
                    
            }
        }
    free(command);
    }
    free(base);
    
    return 0;

}
