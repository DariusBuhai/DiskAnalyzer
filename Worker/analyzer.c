#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "analyzer.h"
#include "../Shared/shared.h"

int total_rec_count = 0;

int is_dir(const char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

long long get_file_size(const char *path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return path_stat.st_size;
}

void set_next_path(const char* base_path, const char* name, char *next_path) {
  strcpy(next_path, base_path);
  if (next_path[strlen(next_path) -1] != '/') {
    strcat(next_path, "/");
  }
  strcat(next_path, name);
}

void count_dirs(const char* base_path, long long *total_size) {

  DIR *dir = opendir(base_path);
  if (dir == NULL) return;

  struct dirent *dp;
  char next_path[MAX_PATH_LENGTH];

  while (1) {
    dp = readdir(dir);
    if (dp == NULL) break;

    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
      set_next_path(base_path, dp->d_name, next_path);
      *total_size += get_file_size(next_path);
      total_rec_count += 1;
      if (is_dir(next_path)) {
        count_dirs(next_path, total_size);
      }
    }
  }
  closedir(dir);
}

/* int process_individual_dir_size(const char* base_path, long long* dir_size, int* index) { */
/*   // this overflows for large computations */
/*  */
/*   DIR *dir = opendir(base_path); */
/*   if (dir == NULL) return 0; */
/*  */
/*   int cur_index = *index; */
/*   struct dirent *dp; */
/*   dir_size[cur_index] = 0; */
/*   char next_path[MAX_PATH_LENGTH]; */
/*  */
/*   while (1) { */
/*     dp = readdir(dir); */
/*     if (dp == NULL) break; */
/*  */
/*     if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) { */
/*       dir_size[cur_index] += get_file_size(next_path); */
/*       set_next_path(base_path, dp->d_name, next_path); */
/*       if (is_dir(next_path)) { */
/*         *index += 1; */
/*         dir_size[cur_index] += dir_size[process_individual_dir_size(next_path, dir_size, index)]; */
/*       } */
/*     } */
/*   } */
/*  */
/*   closedir(dir); */
/*   return cur_index; */
/* } */


long long output_data(const char*base_path, char unit[], char hashtags[],
    FILE *analysis_fd, const char* status_path, long long* total, int first,
    int *file_count, int *dir_count, int job_id) {

  DIR *dir = opendir(base_path);
  if (dir == NULL) return 0;

  struct dirent *dp;
  char next_path[MAX_PATH_LENGTH];
  long long cur_dir_size = 0;

  while (1) {
    dp = readdir(dir);
    if (dp == NULL) break;

    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
      set_next_path(base_path, dp->d_name, next_path);
      cur_dir_size += get_file_size(next_path);
      if (is_dir(next_path)) {
        cur_dir_size += output_data(next_path, unit, hashtags, analysis_fd,
            status_path, total, 0, file_count, dir_count, job_id);
        *dir_count += 1;
      }
      else {
        *file_count += 1;
      }
    }
  }
  closedir(dir);

  double percentage = (double)cur_dir_size / (double)*total;
  int hashtag_cnt = percentage * 40.0 + 1;
  if (hashtag_cnt > 40) hashtag_cnt = 40;

  long long copy = cur_dir_size;
  unit = "B";
  if (copy > 1024) {
    unit = "KB";
    copy /= 1024;
  }
  if (copy > 1024) {
    unit = "MB";
    copy /= 1024;
  }
  if (copy > 1024) {
    unit = "GB";
    copy /= 1024;
  }

  for (int i = 0; i < hashtag_cnt; ++ i) {
    hashtags[i] = '#';
  }
  hashtags[hashtag_cnt] = '\0';

  if (!first) {
    fprintf(analysis_fd, "|-%s/  %0.2lf%%  %lld%s  %s\n", base_path, 
        percentage * 100, copy, unit, hashtags);
  }

  FILE* status_fd = safe_fopen(status_path, "w", job_id);
  if (status_fd) {
    percentage = (double) (*file_count + *dir_count) / (double) total_rec_count;
    fprintf(status_fd, "%d%%\n%d files\n%d dirs", 
        (int)(percentage * 100), *file_count, *dir_count);
    safe_fclose(status_fd, job_id);
  }

  //sleep(1);
  return cur_dir_size;
}

// TODO lockfiles
void analyze(const char* path, int job_id) {

    char* output_path = get_current_path();
    char* status_path = get_current_path();

    strcat(output_path, ANALYSIS_PATH);
    strcat(status_path, STATUS_PATH);

    sprintf(output_path, output_path, job_id);
    sprintf(status_path, status_path, job_id);

  FILE *fd = safe_fopen(output_path, "w", job_id);
  if (fd == NULL) {
    fprintf(stderr, "Path doesn't exist!\n");
    return;
  }

  long long total_size = 0;
  count_dirs(path, &total_size);

  char unit[3], hashtags[42];
  long long copy = total_size;
  unit[0] = 'B'; unit[1] = '\0';
  if (copy > 1024) {
    unit[0] = 'K'; unit[1] = 'B'; unit[2] = '\0';
    copy /= 1024;
  }
  if (copy > 1024) {
    unit[0] = 'M';
    copy /= 1024;
  }
  if (copy > 1024) {
    unit[0] = 'G';
    copy /= 1024;
  }

  for (int i = 0; i < 41; ++ i) {
    hashtags[i] = '#';
  }

  fprintf(fd, "Path  Usage  Size  Amount\n%s  100%%  %lld%s  %s\n|\n",
      path, copy, unit, hashtags);

  int file_count = 0, dir_count = 0;
  output_data(path, unit, hashtags, fd, status_path,
      &total_size, 1, &file_count, &dir_count, job_id);

  safe_fclose(fd, job_id);
}
