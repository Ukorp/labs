#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int is_int(char * str){
    
    for (int i = 0; i < strlen(str); i++){
        if ((str[i] == '-') && (i == 0)) continue;
        if (str[i] < '0' || str[i] > '9') 
            return 0;
    }
    return 1;
}

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

int Nto10(char * in, int base){
    int ans, pw;
    pw = 1;
    int tmp;
    ans = 0;
    int size = strlen(in);
    reverse(in);
    for (int i = 0; i < strlen(in); ++i){
        if (in[i] >= 'A' && in[i] <= 'Z'){
            tmp = in[i] - 'A' + 10;
        }
        else if (in[i] >= 'a' && in[i] <= 'z'){
            tmp = in[i] - 'a' + 10;
        }
        else
            tmp = in[i] - '0';
        ans += tmp * pow(base, i);
    }
    reverse(in);
    return ans;
}

void itobase(int num, int base, char * str){
    int i = 0;
    char num_char;
    if (num == 0) {
        strcpy(str, "0");
        return;
    }
    while (num != 0){
        num_char = (num % base <= 9) ? (num % base) + '0' : (num % base) - 10 + 'a';
        str[i] = num_char;
        num /= base;
        i++;
    }
    str[i] = 0;
    reverse(str);
}

int this_base(char * str, int base){
    char k;
    for (int i = 0; i < strlen(str); i++){
        k = str[i];
        if (k >= '0' && k <= '9'){
            if ((k - '0') > (base - 1)) return 0;
        }
        if (k >= 'a' && k <= 'z'){
            if (((k - 'a') + 10) > (base - 1)) return 0;
        }
        return 1;
    }
}

void delete_0(char * str){
    int i, j;
    i = 0;
    j = 0;
    while (str[j] == '0') j++;
    if (str[j] == 0){
        str[0] = '0';
        str[1] = 0;
        return;
    }
    while (str[j] != 0){
        str[i] = str[j];
        i++;
        j++;
    }
    str[i] = 0;

}

int main(){
    int in;
    char * str = (char *)malloc(256);
    char * ans = (char *)malloc(256);
    scanf("%d", &in);
    if ((in > 36) || (in < 2)){
        printf("Введена неверная система счисления\n");
        return 0;
    }
    int count = 0;
    scanf("%s", str);
    while (strcmp(str, "Stop") != 0){
        if (this_base(str, in) == 0){
            printf("%s не может быть числом в системе счисления %d\n", str, in);
        }
        else{
        delete_0(str);
        itobase(Nto10(str, in), 9, ans);
        printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, ans, 9);
        itobase(Nto10(str, in), 18, ans);
        printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, ans, 18);
        itobase(Nto10(str, in), 27, ans);
        printf("%s в %d системе счисления = %s в %d системе счисления\n",str, in, ans, 27);
        itobase(Nto10(str, in), 36, ans);
        printf("%s в %d системе счисления = %s в %d системе счисления\n\n",str, in, ans, 36);
        count++;
        }
        scanf("%s", str);
    }
    free(str);
    free(ans);

    
}