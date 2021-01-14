#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>


#include "shared.h"

LD bytes_to_xb(LL bytes, int x){
    LD xb = (LD)bytes;
    for(int i=0;i<x;++i)
        xb/=1024;
    return xb;
}

void write_to_file(char* file_location, char data[]){
    struct stat buffer;
    if(stat(file_location, &buffer)>=0)
        remove(file_location);
    int tf = open(file_location, O_CREAT | O_WRONLY, S_IRWXU);
    if(write(tf, data, sizeof (char)*strlen(data)) <= 0)
        perror(NULL);
    close(tf);
}

char* read_from_file(char* file_location){
    struct stat buffer;
    if(stat(file_location, &buffer)<0){
        printf("No file available!\n");
        return 0;
    }
    FILE* fp = fopen(file_location, "r");
    char* data = malloc(sizeof(char)*buffer.st_size);

    fread(data, buffer.st_size, 1, fp);
    return data;
}

char* get_literal_priority(int priority){
    if(priority==1)
        return "low";
    if(priority==2)
        return "normal";
    return "high";
}

