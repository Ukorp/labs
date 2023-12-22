#define getchar(void) getc(stdin)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char * value;
    struct Node * son, * bro;
}Node;

typedef struct Tree{
    Node * current, *prev;
    Node * first;
}Tree;

Node * create_node(char * value){
        Node * node = malloc(sizeof(Node *));
        if (value != NULL)
            node->value = strdup(value);
        node->son = NULL;
        node->bro = NULL;
        return node;
}

void change_value(Node * node, char * value){
    free(node->value);
    node->value = strdup(value);
}

Node add_son(Node * node,char * value){
    node->son = create_node(value);
}

Node add_bro(Node * node,char * value){
    node->son = create_node(value);
}

void print_tree(Tree * tree){
    tree->current = tree->first;
    static int i = 0;
    print_node(tree->current);
}

void print_node(Node * node){
    static int spaces = 0;
    if (node->bro != NULL){
        print_node(node->bro);
        node->bro == NULL;
    }
    else if (node->son != NULL){
        spaces++;
        print_node(node->son);
        spaces--;
        node->son == NULL;
    }
    for (int i = 0; i < spaces; ++i) printf("\t");
    printf("%s\n", node->value);
}

void fill_tree(Node * node){
    char k = getchar();
    static char str[100];
    static int i = 0;
    if (k == '\n') return;
    if (k == '('){
        str[i] = 0;
        i = 0;
        if (*str != 0){
            change_value(node, str);
        }
        node->son = create_node(NULL);
        fill_tree(node->son);
        fill_tree(node);

    }
    else if (k == ')'){
        str[i] = 0;
        i = 0;
        if (*str != 0){
            change_value(node, str);
        }
        fill_tree(node);
    }
    else if (k == ','){
        str[i] = 0;
        i = 0;
        if (*str != 0){
            change_value(node, str);
        }
        node->bro = create_node(NULL);
        fill_tree(node->bro);
        fill_tree(node);
    }
    else{
        str[i++] = k;
        fill_tree(node);
    }
}

int main()
{
    char k;
    Tree * tree = malloc(sizeof(Tree *));
    char string[100];
    int i = 0;
    tree->first = create_node(NULL);
    tree->current = tree->first;
    fill_tree(tree->current);
    // while ((k = getchar()) != '\n'){
    //     if (k == '('){
    //         string[i] = 0;
    //         if (i != 0){
    //             change_value(tree->current, string);
    //         }
    //         i = 0;
    //         tree->current->son = create_node(NULL);
    //         tree->prev = tree->current;
    //         tree->current = tree->current->son;
    //     }
    //     else if (k == ')'){
    //         string[i] = 0;
    //         if (i != 0){
    //             change_value(tree->current, string);
    //         }
    //         i = 0;
    //         tree->current = tree->prev;
    //     }
    //     else if (k == ','){
    //         string[i] = 0;
    //         if (i != 0){
    //         change_value(tree->current, string);
    //         }
    //         i = 0;
    //         tree->current->bro = create_node(NULL);
    //         // tree->prev = tree->current;
    //         tree->current = tree->current->bro;
    //     }
    //     else {
    //         string[i++] = k;
    //     }
        
    // }
    print_tree(tree);

    return 0;
}
