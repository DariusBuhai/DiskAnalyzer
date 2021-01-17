#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "shared.h"

LD bytes_to_xb(LL bytes, int x){
    LD xb = (LD)bytes;
    for(int i=0;i<x;++i)
        xb/=1024;
    return xb;
}

int fsize(FILE *fp) {
  fseek(fp, 0, SEEK_END); 
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET); 
  return size;
}

void write_to_file(const char* file_path, const char* data){
  FILE* fp = fopen(file_path, "w");
  if (fp == NULL) {
    fprintf(stderr, "Could not open output file\n");
    return;
  }

  fprintf(fp, "%s", data);
  fclose(fp);
}

char* read_from_file(const char* file_path){
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
      fprintf(stderr, "Could not open input file\n");
      return NULL;
    }

    int file_size = fsize(fp);
    char* data = malloc(sizeof(char) * file_size);
    fread(data, file_size, 1, fp);
    fclose(fp);

    return data;
}

char* get_literal_priority(int priority){
    if (priority == 1)
        return "low";
    if (priority == 2)
        return "normal";
    if (priority == 3)
        return "high";
    return "unknown";
}

char* get_literal_status(int status) {
    if (status == T_PENDING) return "pending";
    if (status == T_IN_PROGRESS) return "in progress";
    if (status == T_PAUSED) return "paused";
    if (status == T_REMOVED) return "killed";
    if (status == T_DONE) return "done";
    return "unknown";
}

char* get_current_path(){
    char* path = malloc(sizeof(char) * FILENAME_MAX);
    CURRENT_DIR(path, FILENAME_MAX);
    return path;
}
