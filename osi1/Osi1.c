#include <stdio.h>

enum errors{
    open_file_error
};

void print_bytes(FILE * file, char * buffer){
    printf("Byte: %d\n", buffer[0]);
    printf("File position indicator: %ld\n", ftell(file));
    printf("Error indicator: %d\n", ferror(file));
    printf("End-of-file indicator: %d\n", feof(file));
}

int main(int argc, char *argv[]) {
    FILE *file;
    char buffer[11];
    int i;
    if (argc != 2){
        puts("Неверное число аргументов");
        return open_file_error;
    }
    if (!(file = fopen(argv[1], "wb"))){
        printf("Open file error\n");
        return open_file_error;
    }
    char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(bytes, sizeof(char), sizeof(bytes), file);
    fclose(file);

    if (!(file = fopen(argv[1], "rb"))){
        printf("Open file error\n");
        return open_file_error;
    }
    while (fread(buffer, sizeof(char), 1, file)) {
        print_bytes(file, buffer);
    }
    fclose(file);

    if (!(file = fopen(argv[1], "rb"))){
        printf("Open file error\n");
        return open_file_error;
    }
    fseek(file, 3, SEEK_SET);
    fread(buffer, sizeof(char), 4, file);
    fclose(file);

    printf("Buffer contents: ");
    for (i = 0; i < 4; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    return 0;
}
