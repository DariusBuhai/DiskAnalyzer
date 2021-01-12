#include <stdlib.h>
#include <string.h>

#include "temp_data.h"

#define LL long long

struct file_node* new_node(char *file){
    struct file_node *p;
    p = malloc(sizeof(struct file_node));
    p->path = malloc(sizeof(char)*2048);
    strcpy(p->path, file);
    p->no_childs = 0;
    p->percentage = 0;
    p->size = 0;
    p->is_dir = 0;
    return p;
}

struct file_node* insert_child(struct file_node *root, struct file_node *child){
    if(root==NULL)
        return NULL;
    root->childs[root->no_childs] = child;
    root->size += child->size;
    root->no_childs++;
    return root;
}