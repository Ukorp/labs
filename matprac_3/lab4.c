#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum errors{
    ok, fail, memory_allocation_problem,
    overdue, already_took, no_adress
}errors;

typedef struct String{
    char * data;
    int size;
}String;

typedef struct Adress{
    String city;
    String street;
    int house;
    String corpus;
    int flat;
    String index;
} Adress;

typedef struct Mail{
    Adress adress;
    double weight;
    String id;
    time_t create;
    String created_time;
    time_t delivery;
    String took_time;
} Mail;

typedef struct Post{
    Adress * adress;
    Mail * mail;
    int size;
}Post;

int comp(const Mail *, const Mail *);

String create_string(char * val){
    String str;
    str.size = strlen(val);
    if (val == NULL || str.size == 0){
        str.data = NULL;
        str.size = -1;
        return str;
    }
    str.data = (char *)malloc(sizeof(char) * (strlen(val) + 1));
    if (str.data == NULL) {
        str.size = -1;
        return str;
    }
    strcpy(str.data, val);
    return str;
}

void free_string(String * str){
    if (str->data == NULL) return;
    str->size = 0;
    free(str->data);
}

int compareStrings(String str1, String str2){
    if (str1.size > str2.size) return 1;
    if (str1.size < str2.size) return -1;
    return strcmp(str1.data, str2.data);
}

int equalStrings(String str1, String str2){
    if (strcmp(str1.data, str2.data) == 0) return 0;
    return 1;
}

errors copyString(String * from, String * to){
    to->size = from->size;
    free(to->data);
    to->data = strdup(from->data);
    if (to->data == NULL) return fail;
    return ok;
}

int isDouble(char * str){
    int n = 0;
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '.') && (n == 0) && (i != 0) && (i != (strlen(str) - 1))) n++;
        else if (str[i] < '0' || str[i] > '9')
            return fail;
        
    }
    return ok;
}


String newCopyString(String * from){
    String new;
    new.size = from->size;
    new.data = (char *)malloc(sizeof(char) * (new.size + 1));
    strcpy(new.data, from->data);
    return new;
}

errors concatenationString(String * str1, String * str2){
    int new_size = str1->size + str2->size;
    char * ptr = (char *)realloc(str1->data, sizeof(char) * (new_size + 1));
    if (ptr == NULL) return memory_allocation_problem;
    for (int i = 0; i < str2->size; i++){
        str1->data[i + str1->size] = str2->data[i];
    }
    str1->data[new_size] = '\0';
    str1->size = new_size;
    return ok;
}

Post * create_post(int size){
    Post * post = (Post *)malloc(sizeof(Post));
    if (post == NULL) return post;
    post->mail = (Mail *)malloc(sizeof(Mail) * size);
    if (post->mail == NULL){
        free(post);
        return NULL;
    }
    post->adress = (Adress *)malloc(sizeof(Adress));
    if (post->adress == NULL){
        free(post->mail);
        free(post);
        return NULL;
    }
    post->size = 0;
    return post;
}

int isInt(String str){
    
    for (int i = 0; i < str.size; i++){
        if ((str.data[i] == '-') && (i == 0)) continue;
        if (str.data[i] < '0' || str.data[i] > '9') 
            return fail;
    }
    return ok;
}

int isIntChar(char * str){
    
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return fail;
    }
    return ok;
}

errors fillPostAdress(Post * post){
    char container[1024];
    int house, flat;
    printf("Введите город почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String city = create_string(container);
    if (city.data == NULL){
        free_string(&city);
        return fail;
    }
    copyString(&city, &post->adress->city);
    free_string(&city);

    printf("Введите улицу почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String street = create_string(container);

    if (street.data == NULL){
        free_string(&street);
        return fail;
    }
    copyString(&street, &post->adress->street);
    free_string(&street);

    printf("Введите номер дома почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (isIntChar(container) != ok) return fail;
    house = atoi(container);
    if (house == 0) return fail;
    post->adress->house = house;

    printf("Введите корпус почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String corpus = create_string(container);
    if (corpus.data == NULL) {
        free_string(&corpus);
        return fail;
    }
    copyString(&corpus, &post->adress->corpus);
    free_string(&corpus);

    printf("Введите номер квартиры почтового отделения: ");
    // scanf("%d", &flat);
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (isIntChar(container) != ok) return fail;
    flat = atoi(container);
    if (flat == 0) return fail;
    post->adress->flat = flat;

    printf("Введите индекс почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String index = create_string(container);
    if (index.data == NULL){
        free_string(&index);
        return fail;
    }
    if (index.size != 6) {
        free_string(&index);
        return fail;
    }
    if (isInt(index) != ok){
        free_string(&index);
        return fail;
    }
    copyString(&index, &post->adress->index);
    free_string(&index);

    return ok;
}

errors fillAdress(Adress * adress){
    char container[1024];
    int house, flat;
    printf("Введите город почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String city = create_string(container);
    if (city.data == NULL){
        free_string(&city);
        return fail;
    }
    copyString(&city, &adress->city);
    free_string(&city);

    printf("Введите улицу почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String street = create_string(container);

    if (street.data == NULL){
        free_string(&street);
        return fail;
    }
    copyString(&street, &adress->street);
    free_string(&street);

    printf("Введите номер дома почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (isIntChar(container) != ok) return fail;
    house = atoi(container);
    if (house == 0) return fail;
    adress->house = house;

    printf("Введите корпус почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String corpus = create_string(container);
    if (corpus.data == NULL) {
        free_string(&corpus);
        return fail;
    }
    copyString(&corpus, &adress->corpus);
    free_string(&corpus);

    printf("Введите номер квартиры почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (isIntChar(container) != ok) return fail;
    flat = atoi(container);
    if (flat == 0) return fail;
    adress->flat = flat;

    printf("Введите индекс почтового отделения: ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    String index = create_string(container);
    if (index.data == NULL){
        free_string(&index);
        return fail;
    }
    if (index.size != 6) {
        free_string(&index);
        return fail;
    }
    if (isInt(index) != ok){
        free_string(&index);
        return fail;
    }
    copyString(&index, &adress->index);
    free_string(&index);

    return ok;
}

// errors fillAdress(Adress * adress){
//     char container[1024];
//     int house, flat;
//     printf("Введите город получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (container == NULL) return fail;
//     adress->city = create_string(container);
//     if (adress->city.data == NULL) return fail;

//     printf("Введите улицу получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (container == NULL) return fail;
//     adress->street = create_string(container);
//     if (adress->street.data == NULL) return fail;

//     printf("Введите номер дома получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (isIntChar(container) != ok) return fail;
//     house = atoi(container);
//     if (house == 0) return fail;
//     adress->house = house;

//     printf("Введите корпус получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (container == NULL) return fail;
//     adress->corpus = create_string(container);
//     if (adress->corpus.data == NULL) return fail;

//     printf("Введите номер квартиры получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (isIntChar(container) != ok) return fail;
//     flat = atoi(container);
//     if (flat == 0) return fail;
//     adress->flat = flat;

//     printf("Введите индекс получателя: ");
//     fgets(container, 1024, stdin);
//     container[strlen(container) - 1] = 0;
//     if (container == NULL) return fail;
//     adress->index = create_string(container);
//     if (adress->index.data == NULL) return fail;
//     if (adress->index.size != 6) return fail;
//     if (isInt(adress->index) != ok) return fail;

//     return ok;
// }

int compareTime(String time1, String time2){
    char * tmp1, *tmp2;
    int num;
    int ans = 0;
    tmp1 = strtok(time1.data, ": ");
    tmp1 = strtok(time2.data, ": ");
    


}

errors fillMail(Mail * mail){
    if (fillAdress(&mail->adress) == fail)
        return fail;
    double weight;
    char container[1024];
    printf("Введите вес посылки (кг): ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (isDouble(container) != ok) return fail;
    weight = atof(container);
    if (weight == 0) return fail;
    mail->weight = weight;

    printf("Введите id (14 цифр): ");
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = 0;
    if (container == NULL) return fail;
    mail->id = create_string(container);
    if (mail->id.data == NULL) return fail;
    if (mail->id.size != 14) return fail;
    if (isInt(mail->id) != ok) return fail;

    time_t time_now = time(NULL);
    struct tm * date = localtime(&time_now);
    char time_string[20];
    snprintf(time_string, 20, "%02d:%02d:%04d %02d:%02d:%02d", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec);
    mail->created_time = create_string(time_string);
    if (mail->created_time.data == NULL) return fail;
    mail->create = time_now;

    mail->took_time = create_string("Не получено");

    return ok;

}

Mail * find_mail(Post * post, String PostId){
    if (PostId.size != 14) return NULL;
    for (int i = 0; i < post->size; ++i){
        if (!equalStrings(PostId, post->mail[i].id)){
            return &post->mail[i];
        }
    }
    return NULL;
}

void free_adress(Adress * adress){
    free_string(&adress->city);
    free_string(&adress->street);
    free_string(&adress->corpus);
    free_string(&adress->index);
}

errors free_mail(Mail * mail){
    free_adress(&mail->adress);
    free_string(&mail->id);
    free_string(&mail->took_time);
    free_string(&mail->created_time);
}



errors delete_mail(Post * post, String id){
    Mail * mail = find_mail(post, id);
    if (mail == NULL) return fail;
    else {
        free_mail(mail);
        free(mail);
    }
    qsort(post->mail, post->size, sizeof(Mail), (int (*) (const void *, const void *)) comp);
    post->size--;
}

void print_adress(Adress * mail){
    printf("Город: %s\n", mail->city);
    printf("Улица: %s\n", mail->street);
    printf("Номер дома: %d\n", mail->house);
    printf("Корпус: %s\n", mail ->corpus);
    printf("Номер квартиры: %d\n", mail->flat);
    printf("Почтовый индекс: %s\n", mail->index);
}

void print_mail(Mail * mail){
    print_adress(&mail->adress);
    printf("Вес посылки: %.3f кг\n", mail->weight);
    printf("id получателя: %s\n", mail->id);
    printf("Время создания запроса: %s\n", mail->created_time);
    printf("Время получения посылки: %s\n", mail->took_time);
}

int print_delivered_mails(Post * post){
    int n = 1;
    for (int i = 0; i < post->size; ++i){
        if  (strcmp(post->mail[i].took_time.data, "Не получено") != 0){
            print_mail(&post->mail[i]);
            puts("\n");
            n = 0;
        }
    }
    return n;
}

errors takeMail(Mail * mail){
    if (strcmp(mail->took_time.data, "Не получено") == 0){
        time_t Time = time(NULL);
        mail->delivery = Time;
        if (difftime(Time, mail->create) > 60*60*24*7){
            free_string(&mail->took_time);
            mail->took_time = create_string("Просрочено");
            return overdue;
        }
        else {
            struct tm * date = localtime(&Time);
            char time_string[20];
            snprintf(time_string, 20, "%02d:%02d:%04d %02d:%02d:%02d", date->tm_mday, date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec);
            mail->took_time = create_string(time_string);
            return ok;
        }
    }
    else {
        return already_took;
    }
}

void print_menu(){
    puts("\tМЕНЮ");
    puts("1 - Ввести адресс почты");
    puts("2 - Отослать посылку");
    puts("3 - Забрать посылку");
    puts("4 - Вывести информацию о посылке");
    puts("5 - Вывести информацию о просроченных и полученных посылках");
    puts("6 - Удалить посылку");
    puts("7 - Выход");
}

void delete_post(Post * post, errors * adress_code){
    if (*adress_code == ok)
        free_adress(post->adress);
    free(post->adress);
    for (int i = 0; i < post->size; ++i){
        Mail * tmp = &post->mail[i];
        free_mail(tmp);
        free(tmp);
    }
    free(post->mail);
    free(post);
}

int main(){
    
    errors adress_code = no_adress;
    int size;
    Post * post = create_post(1);
    Mail * tmp_mail;
    int i = 0;
    int num;
    char stringInput[1024];
    char container[1024];
    while (1){
    print_menu();
    fgets(container, 1024, stdin);
    container[strlen(container) - 1] = '\0';
    if (isIntChar(container) != ok) {
        puts("Команда не найдена");
        continue;
    }
    num = atoi(container);
    switch (num){
        case 1:
            if (fillPostAdress(post) == fail){
                puts("Неверно введён адресс\n");
                free_adress(post->adress);
                break;
            };
            adress_code = ok;
            puts("Успешно!\n");
            break;
        case 2:
            if (fillMail(&post->mail[i++]) == fail){
                puts("Неверно введены данные посылки");
                delete_mail(post, post->mail[--i].id);
                break;
            };
            
            tmp_mail = (Mail *)realloc(post->mail, sizeof(Mail) * (i + 1));
            if (tmp_mail == NULL){
                puts("Ошибка выделения памяти");
                delete_mail(post, post->mail[--i].id);
                break;
            }
            
            post->mail = tmp_mail;
            post->size = i;
            puts("Успешно!");
            break;
        case 3:
            puts("Введите id: ");
            fgets(stringInput, 1024, stdin);
            stringInput[strlen(stringInput) - 1] = '\0';
            String tmp = create_string(stringInput);
            if (tmp.data == NULL) return fail;
            Mail * mail = find_mail(post, tmp);
            free_string(&tmp);
            if (mail == NULL){
                puts("Посылка не найдена");
                break;
            }
            
            switch(takeMail(mail)){
                case ok:
                    puts("Успешно!");
                    break;
                case overdue:
                    puts("Посылка просрочена");
                    break;
                case already_took:
                    puts("Посылка уже получена");
                    break;
            }
            break;
        case 4:
            puts("Введите id: ");
            fgets(stringInput, 1024, stdin);
            stringInput[strlen(stringInput) - 1] = '\0';
            String tmp1 = create_string(stringInput);
            if (tmp1.data == NULL) return fail;
            Mail * mail1 = find_mail(post, tmp1);
            if (mail1 == NULL){
                puts("Посылка не найдена\n");
                break;
            }
            print_mail(mail1);
            break;
        case 5:
            if (print_delivered_mails(post)) puts("Полученных посылок не найдено\n");
            break;
        case 6:
            puts("Введите id: ");
            fgets(stringInput, 1024, stdin);
            stringInput[strlen(stringInput) - 1] = '\0';
            String tmp2 = create_string(stringInput);
            if (tmp2.data == NULL) return fail;
            Mail * mail2 = find_mail(post, tmp2);
            free_string(&tmp2);
            if (mail2 == NULL){
                puts("Посылка не найдена\n");
                break;
            }
            delete_mail(post, tmp2);
            tmp_mail = (Mail *)realloc(post->mail, sizeof(Mail) * i--);
            if (tmp_mail == NULL){
                puts("Ошибка выделения памяти\n");
                break;
            }
            post->mail = tmp_mail;
            post->size = i;
            puts("Успешно!\n");
            break;
        case 7:
            delete_post(post, &adress_code);
            return ok;
        default:
            puts("Команда не найдена\n");
            break;
    }
    }

}

int comp(const Mail * one, const Mail * two){
    if (one == NULL) return 1;
    if (two == NULL) return -1;
    if (compareStrings(one->adress.index, two->adress.index) != 0) return compareStrings(one->adress.index, two->adress.index);
    return compareStrings(one->id, two->id);
}