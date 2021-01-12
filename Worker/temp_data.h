#ifndef DISKANALYZER_TEMP_DATA_H
#define DISKANALYZER_TEMP_DATA_H

#define LL long long

struct file_node{
    char* path;
    int no_childs, is_dir;
    double percentage;
    LL size;
    struct file_node *childs[5048];
};

struct file_node* new_node(char*);
struct file_node* insert_child(struct file_node *, struct file_node *);

#endif DISKANALYZER_TEMP_DATA_H
