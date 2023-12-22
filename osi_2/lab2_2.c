#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
    fork();
    printf("Hi\n");
    int id = fork();
    if (id == 0)
    {
        printf("Hi\n");
    }
    else
    {
        printf("Hi\n");
    }
    
    printf("Hi\n"); 
    printf("Hi\n");
}