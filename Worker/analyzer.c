#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

#include "temp_data.h"
#include "Shared/shared.h"

struct file_details* save_dirs(struct file_details* location, int *total_tasks){
    struct dirent* file;
    struct stat buffer;
    DIR* dir_path = opendir(location->path);
    (*total_tasks)++;

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
            #if SHOW_ERRORS
                perror(file->d_name);
            #endif
            continue;
        }

        char new_path[2048];
        snprintf(new_path, sizeof(new_path), "%s/%s", location->path, file->d_name);

        struct file_details* new_location = new_file_details(new_path);

        new_location->size = buffer.st_size;
        if(file->d_type==DT_DIR){
            new_location->is_dir = 1;
            save_dirs(new_location, total_tasks);
        }
        append_file(location, new_location);
    }
    closedir(dir_path);
    return location;
}

struct file_details* calculate_dirs(struct file_details* location, int *done_tasks){
    struct file_details* current;
    (*done_tasks)++;
    for(int i=0;i<location->childs_counter;++i){
        current = location->childs[i];
        current->usage = (double)current->size / (double)location->size;

        char af[101] = "";
        for(int j=0;j<current->usage*100;++j)
            strcat(af, "#");
        #if DEBUG
            //printf("%s %.2f %.3LfMB %s\n", current->path, current->usage*100, bytes_to_xb(current->size, 2), af);
        #endif
        if(current->is_dir)
            calculate_dirs(current, done_tasks);
    }
    return location;
}

void* analyze_dir(char* path, struct process_details* process){
    process->done_tasks = 0;
    process->total_tasks = 0;
    process->status = RUNNING;

    struct file_details* location = new_file_details(path);
    save_dirs(location, &process->total_tasks);
    location->usage = 1;
    calculate_dirs(location, &process->done_tasks);

    process->response = (void*)location;
    process->status = DONE;

    pthread_exit((void *)location);
}
