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

void swap(int * arr, int size, int a, int b){
    int max = a-1;
    int min = b + 1;
    int index_max, index_min;
    index_max = 0;
    index_min = 0;
    for(int i = 0; i < size; ++i){
        if (arr[i] > max){
            max = arr[i];
            index_max = i;
        }
        if (arr[i] < min){
            min = arr[i];
            index_min = i;
        }
    }
    int temp = arr[index_min];
    arr[index_min] = arr[index_max];
    arr[index_max] = temp; 
}

int main(int argc, char * argv[]){
    if ((is_int(argv[1]) && is_int(argv[2])) == 0){
        printf("Неверно поданы аргументы\n");
        return 1;
    }
    int a, b;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    unsigned int stime = (unsigned int)time(NULL) / 2;
    srand(stime);
    const int size = 250;
    int arr[size];
    for (int i = 0; i < 250; ++i){
        arr[i] = (a + rand() % (b - a + 1));
    }
    swap(&(*arr), size, a, b);

}