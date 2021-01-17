#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

static char* current_path;

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
    if (status == PENDING) return "pending";
    if (status == PROCESSING) return "in progress";
    if (status == PAUSED) return "paused";
    if (status == REMOVED) return "killed";
    if (status == DONE) return "done";
    return "unknown";
}

void save_current_path(){
    current_path = calloc(FILENAME_MAX, sizeof(char));
    #ifdef CLION
        current_path = "/Users/dariusbuhai/Desktop/Programs/C/DiskAnalyzer";
    #else
        CURRENT_DIR(current_path, FILENAME_MAX);
    #endif
}

char* get_current_path(){
    char* path = calloc(FILENAME_MAX, sizeof(char));
    strcat(path, current_path);
    return path;
}

FILE* safe_fopen(const char* file_name, const char* file_perm, int task_id) {
  char aux[20];
  sprintf(aux, LOCK_PATH, task_id);
  while (access(aux, F_OK) == 0);
  open(aux, O_WRONLY | O_APPEND | O_CREAT, 0644);

  FILE *out = fopen(file_name, file_perm);
  return out;
}

void safe_fclose(FILE *file_name, int task_id) {
  fclose(file_name);

  char aux[20];
  sprintf(aux, LOCK_PATH, task_id);
  if (access(aux, F_OK) == 0) {
    remove(aux);
  }
}

int is_prefix(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? 0 : memcmp(pre, str, lenpre) == 0;
}
