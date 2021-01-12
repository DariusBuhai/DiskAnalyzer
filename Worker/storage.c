#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "temp_data.h"

#define LL long long
#define LD long double

LD bytes_to_xb(LL bytes, int x){
    LD xb = (LD)bytes;
    for(int i=0;i<x;++i)
        xb/=1024;
    return xb;
}

struct file_node* save_dir_paths(struct file_node* location){
    struct dirent* file;
    struct stat buffer;
    DIR* dir_path = opendir(location->path);

    if (dir_path == NULL){
        perror("opendir");
        return NULL;
    }
    if (stat(location->path, &buffer) < 0){
        perror(location->path);
        return NULL;
    }
    location->size = buffer.st_size;

    while((file = readdir(dir_path)) != NULL){
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            continue;
        if (fstatat(dirfd(dir_path), file->d_name, &buffer, 0) < 0){
            perror(file->d_name);
            continue;
        }

        char new_path[2048];
        snprintf(new_path, sizeof(new_path), "%s/%s", location->path, file->d_name);

        struct file_node* new_location = new_node(new_path);

        new_location->size = buffer.st_size;
        if(file->d_type==DT_DIR){
            new_location->is_dir = 1;
            save_dir_paths(new_location);
        }
        insert_child(location, new_location);
    }
    closedir(dir_path);
    return location;
}

struct file_node* calculate_dir_perc(struct file_node* location){
    struct file_node* current;
    for(int i=0;i<location->no_childs;++i){
        current = location->childs[i];
        current->percentage = (double)current->size / (double)location->size;

        char af[101] = "";
        for(int i=0;i<current->percentage*100;++i)
            strcat(af, "#");
        printf("%s %.2f %.3LfMB %s\n", current->path, current->percentage*100, bytes_to_xb(current->size, 2), af);
        if(current->is_dir)
            calculate_dir_perc(current);
    }
    return location;
}

LL analyze_dir(char* path){
    struct file_node* location = new_node(path);
    save_dir_paths(location);
    location->percentage = 1;
    calculate_dir_perc(location);
    return location->size;
}
