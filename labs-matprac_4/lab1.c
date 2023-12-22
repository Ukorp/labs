#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char arr[62] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWZYZabcdefghijklmnopqrstuvwxyz";

typedef enum {
    OK, OPEN_FILE_ERROR, MEMORY_ALLOCATION_PROBLEM,
    DEFINE_ERROR
} errors;

typedef struct HashNode{
    char *name;
    char *value;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode ** table;
    int size;
} HashTable;


HashTable * create_HashTable(int HashSize){
    HashTable * table = (HashTable *)malloc(sizeof(HashTable));
    table->size = HashSize;
    table->table = (HashNode **)malloc(sizeof(HashNode) * HashSize);
    for (int i = 0; i < HashSize; ++i){
        table->table[i] = NULL;
    }
    return table;
}

int hash_function(const char *name, int table_size) {
    unsigned long hash = 0;
    char tmp;
    for (int i = 0; i < strlen(name); ++i) {
        if (name[i] >= 'A' && name[i] <= 'Z') tmp = name[i] - 'A' + 10;
        else if (name[i] >= 'a' && name[i] <= 'z') tmp = name[i] - 'a' + 36;
        else tmp = name[i] - '0';
        hash = ((hash * 62) + tmp) % table_size;
    }
    return hash;
}

void insert_hash(HashTable * table, const char * name, const char * value){
    int hash_value = hash_function(name, table->size);
    HashNode * node = table->table[hash_value];
    while (node != NULL){
        if (strcmp(node->name, name) == 0){
            free(node->value);
            node->value = strdup(value);
            return;
        }
        node = node->next;
    }
    HashNode * new_node = (HashNode *)malloc(sizeof(HashNode *));
    new_node->name = strdup(name);
    new_node->value = strdup(value);
    new_node->next = table->table[hash_value];
    table->table[hash_value] = new_node;
    return;
}

char * str_from_file(FILE * input_file, FILE * output_file ,char * str){
    char k;
    int i = 0;
    while ((k = getc(input_file)) != ' ' && k != '\t' && k != '\n' && k != EOF){
        char k;
    }
}

errors replace(char * input, char * output){
    FILE * input_file, * output_file;
    if ((input_file = fopen(input, "r")) == NULL) return OPEN_FILE_ERROR;
    if ((output_file = fopen(output, "w")) == NULL){
        fclose(input_file);
        return OPEN_FILE_ERROR;
    }
    int size = 128;
    HashTable * table = create_HashTable(size);
    char str[1000];
    char * token = (char *)malloc(sizeof(char) * 1000);
    char * val = (char *)malloc(sizeof(char) * 1000);
    char k;
    int i = 0;
    int define_number = 0;

    while (fgets(str, 1000, input_file)){
        token = strtok(str, " \t\n");
        do{
            
            if (token == NULL) break;
            if (strcmp(token, "#define") == 0){
                fprintf(output_file, "%s ", token);
                token = strtok(NULL, " \t\n");
                fprintf(output_file, "%s ", token);
                val = strtok(NULL, " \t\n");
                fprintf(output_file, "%s ", val);
                insert_hash(table, token, val);
                token = strtok(NULL, " \t\n");;
            }
            if (token == NULL) break;
            if (table->table[hash_function(token, size)] != NULL){
                if (strcmp(token, table->table[hash_function(token, size)]->name) == 0){
                    fprintf(output_file, "%s ", table->table[hash_function(token, size)]->value);
                }
            }
            else fprintf(output_file, "%s ", token);
            token = strtok(NULL, " \t\n");
        }while (token != NULL);
        fprintf(output_file, "\n");
    }   

}

int main()
{
    switch (replace("hz.txt", "ans.txt"))
    {
    case DEFINE_ERROR:
        printf("1\n");
        break;
    case OPEN_FILE_ERROR:
        printf("2\n");
        break;
    default:
        break;
    }
    
}
