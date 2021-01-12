#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define LL long long
#define LD long double

LL analyze_dir(const char* path){
    LL total_size = 0;

    struct dirent* file;
    struct stat buffer;
    char new_path[2048];
    DIR* dir_path = opendir(path);

    if (dir_path == NULL){
        perror("opendir");
        return -1;
    }

    while((file = readdir(dir_path)) != NULL){
        
        if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            continue;
        if (fstatat(dirfd(dir_path), file->d_name, &buffer, 0) < 0){
            perror(file->d_name);
            continue;
        }

        snprintf(new_path, sizeof(new_path), "%s/%s", path, file->d_name);
        printf("%s %lld bytes\n", new_path, buffer.st_size);

        total_size += buffer.st_size;
        
        if(file->d_type==DT_DIR)
            total_size += analyze_dir(new_path);

    }
    closedir(dir_path);
    return total_size;
}


LD bytes_to_gb(LL bytes){
    LD gb = (LD)bytes;
    for(int i=0;i<3;++i)
        gb/=1024;
    return gb;
}

int main(int argc, char **argv) {
    if(argc!=2)
        return -1;
    char *file = argv[1];
    LL total_size = analyze_dir(file);

    printf("Total size: %lld bytes, %Lf GB\n", total_size, bytes_to_gb(total_size));
    return 0;
}
