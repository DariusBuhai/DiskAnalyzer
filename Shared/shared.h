#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef WINDOWS
    #include <direct.h>
    #define CURRENT_DIR _getcwd
#else
    #include <unistd.h>
    #define CURRENT_DIR getcwd
#endif

/// Settings
#define SHOW_ERRORS
#define ALLOWED_PROCESSES 5

/// Signals paths

#ifdef __APPLE__
    #define FULL_PATH_PREFIX "/Users/dariusbuhai/Desktop/Programs/C/DiskAnalyzer"
#else
    #ifdef WINDOWS
        /// Vlad pune aici path-ul tau, e nevoie pentru daemon
        #define FULL_PATH_PREFIX ""
    #else
        /// Stefan pune path-ul tau aici, e nevoie pentru daemon
        #define FULL_PATH_PREFIX ""
    #endif
#endif


#define PID_PATH "/TempData/daemon.pid"
#define OUTPUT_PATH "/TempData/daemon_output.txt"
#define INSTRUCTION_PATH "/TempData/daemon_instruction.txt"
#define ANALYSIS_PATH "/TempData/analysis_%d"
#define STATUS_PATH "/TempData/status_%d"
#define LOCK_PATH "/TempData/%d.lock"

/// Standards
#define MAX_SUBDIRS 4096
#define MAX_PID_SIZE 10
#define MAX_PATH_LENGTH 1024

#define LL long long
#define LD long double

#define PENDING 1
#define PROCESSING 2
#define PAUSED 3
#define DONE 4
#define REMOVED 5

#define ADD 1
#define SUSPEND 2
#define RESUME 3
#define KILL 4
#define INFO 5
#define LIST 6
#define PRINT 7

/// Data types
struct signal_details {
    int type, priority;
    pid_t pid, ppid;
    char* path;
};

struct file_details {
    char* path;
    int childs_counter, is_dir;
    double usage;
    LL size;
    struct file_details *childs[MAX_SUBDIRS];
};

struct task_details {
  int task_id;
  char path[1024];
  int status;
  int priority;
  int worker_pid;
};

/// Methods
int fsize(FILE *fp);
void write_to_file(const char*, const char* data);
char* read_from_file(const char*);
char* get_literal_priority(int);
char* get_literal_status(int);
char* get_current_path();
FILE* safe_fopen(const char*, const char*, int);
void safe_fclose(FILE*, int);

#endif //DISKANALYZER_SHARED_H
