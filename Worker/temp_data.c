#include <stdlib.h>
#include <string.h>

#include "temp_data.h"

struct file_details* new_file_details(char *file) {
    struct file_details *p;
    p = malloc(sizeof(struct file_details));
    p->path = malloc(sizeof(char)*2048);
    strcpy(p->path, file);
    p->childs_counter = 0;
    p->usage = 0;
    p->size = 0;
    p->is_dir = 0;
    return p;
};

struct file_details* append_file(struct file_details *root, struct file_details *child){
    if(root==NULL)
        return NULL;
    root->childs[root->childs_counter] = child;
    root->size += child->size;
    root->childs_counter++;
    return root;
};
